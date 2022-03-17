#include "SequentialSolverService.h"

namespace core::sat::solver {

void SequentialSolverService::_solver_working_thread(
    core::sat::solver::Solver& solver, util::Timer& timer) {
  while (!_stop) {
    std::unique_lock<std::mutex> ul(_queue_m);
    _queue_cv.wait(ul, [this] { return _stop || !_task_queue.empty(); });
    if (IPS_UNLIKELY(_stop)) {
      break;
    }
    if (IPS_UNLIKELY(_task_queue.empty())) {
      continue;
    }
    auto task = std::move(_task_queue.front());
    _task_queue.pop();
    ul.unlock();
    task(solver, timer);
  }
}

SequentialSolverService::SequentialSolverService(
    SequentialSolverServiceConfig const& config)
    : _timers(config.num_solvers()) {
  for (int i = 0; i < config.num_solvers(); ++i) {
    _solvers.emplace_back(USolver(
        core::sat::solver::SolverRegistry::resolve(config.solver_config())));
    _threads.emplace_back(
        [this, i] { _solver_working_thread(*_solvers[i], _timers[i]); });
  }
}

std::future<core::sat::State> SequentialSolverService::solve(
    Mini::vec<Mini::Lit> const& assumptions,
    core::clock_t::duration time_limit) {
  auto p_task =
      std::packaged_task<core::sat::State(sat::solver::Solver&, util::Timer&)>(
          [assumptions, time_limit](auto& solver, auto& timer) {
            return IPS_TRACE_N_V(
                "SequentialSolverService::solve",
                timer.template launch<State>(
                    [&] { return solver.solve(assumptions); },
                    [&] { solver.interrupt(); }, time_limit));
          });
  auto future = p_task.get_future();
  {
    std::lock_guard<std::mutex> lg(_queue_m);
    _task_queue.emplace(std::move(p_task));
  }
  _queue_cv.notify_one();
  return future;
}

SequentialSolverService::~SequentialSolverService() noexcept {
  {
    std::lock_guard<std::mutex> lg(_queue_m);
    _stop = true;
    _task_queue = {};
  }
  for (auto& solver : _solvers) {
    solver->interrupt();
  }
  _queue_cv.notify_all();
  for (auto& thread : _threads) {
    if (thread.joinable()) {
      thread.join();
    }
  }
}

void SequentialSolverService::interrupt() {
  {
    std::lock_guard<std::mutex> lg(_queue_m);
    _task_queue = {};
  }
  for (auto& solver : _solvers) {
    solver->interrupt();
  }
}

void SequentialSolverService::clear_interrupt() {
  for (auto& solver : _solvers) {
    solver->clear_interrupt();
  }
}

void SequentialSolverService::load_problem(const Problem& problem) {
  for (auto& solver : _solvers) {
    solver->load_problem(problem);
  }
}

REGISTER_PROTO(
    SolverService, SequentialSolverService, sequential_solver_service_config);

}  // namespace core::sat::solver