#include "core/sat/solver/ParSolver.h"

#include <mutex>

#include "util/options.h"
#include "util/stream.h"

namespace core::sat::solver {

ParSolver::ParSolver(ParSolverConfig const& config)
    : _solver_pool(config.max_threads(), [&config] {
      return RSolver(SolverRegistry::resolve(config.solver_config()));
    }) {}

void ParSolver::load_problem(Problem const& problem) {
  auto& workers = _solver_pool.get_workers();
  std::for_each(IPS_EXEC_POLICY, workers.begin(), workers.end(), [&problem](auto& solver) {
    solver->load_problem(problem);
  });
}

State ParSolver::solve(Minisat::vec<Minisat::Lit> const& assumptions) {
  return _solver_pool.get_workers().front()->solve(assumptions);
}

void ParSolver::solve_assignments(domain::USearch search, Solver::slv_callback_t const& callback) {
  clear_interrupt();
  uint32_t num_threads = _solver_pool.max_threads();

  // Here we share assignment between all worker threads.
  domain::RSearch shared_search(search.release());

  std::vector<std::future<void>> futures;
  futures.reserve(num_threads);
  _solve_finished = false;
  for (uint32_t index = 0; index < num_threads; ++index) {
    futures.push_back(_solver_pool.submit([this, &shared_search, &callback](RSolver& solver) {
      _solve(*solver, shared_search, callback);
    }));
  }
  SolverWorkerPool::wait_for(futures);
}

void ParSolver::_solve(
    sat::solver::Solver& solver, domain::RSearch search, slv_callback_t const& callback) {
  if (!search->empty()) {
    auto& assignment = *search;
    Minisat::vec<Minisat::Lit> arg(assignment().size());
    do {
      {
        std::lock_guard<std::mutex> asgn_lg(_asgn_mutex);
        if (_solve_finished) {
          break;
        }
        auto const& arr = assignment();
        for (int i = 0; i < arr.size(); ++i) {
          arg[i] = arr[i];
        }
        if (!++assignment) {
          _solve_finished = true;
        }
      }
      bool conflict = solver.propagate_confl(arg);
      State result = conflict ? UNSAT : solver.solve(arg);
      if (!callback(result, conflict, arg)) {
        break;
      }
    } while (!IPS_UNLIKELY(interrupted()));
  }
}

void ParSolver::_do_interrupt() {
  auto& workers = _solver_pool.get_workers();
  std::for_each(
      IPS_EXEC_POLICY, workers.begin(), workers.end(), [](auto& solver) { solver->interrupt(); });
}

void ParSolver::_do_clear_interrupt() {
  auto& workers = _solver_pool.get_workers();
  std::for_each(IPS_EXEC_POLICY, workers.begin(), workers.end(), [](auto& solver) {
    solver->clear_interrupt();
  });
}

unsigned ParSolver::num_vars() const noexcept {
  return _solver_pool.get_workers().front()->num_vars();
}

bool ParSolver::propagate_confl(Minisat::vec<Minisat::Lit> const& assumptions) {
  return _solver_pool.get_workers().front()->propagate_confl(assumptions);
}

REGISTER_PROTO(Solver, ParSolver, par_solver_config);

}  // namespace core::sat::solver