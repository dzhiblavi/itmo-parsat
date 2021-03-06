#ifndef ITMO_PARSAT_SOLVE_H
#define ITMO_PARSAT_SOLVE_H

#include <iostream>
#include <memory>
#include <atomic>

#include "core/sat/solver/sequential/Solver.h"
#include "util/EventHandler.h"
#include "util/SigHandler.h"
#include "util/TimeTracer.h"
#include "core/proto/solve_config.pb.h"
#include "core/sat/sharing/Shareable.h"

namespace core::solve {

/**
 * @brief The core class that performs all actions related to solving SAT.
 */
class Solve : public sat::sharing::Shareable {
 public:
  Solve() = default;

  virtual ~Solve() = default;

  /**
   * @brief This method solves SAT. For now, it is expected to be called only
   * once per object lifetime.
   * @param input path to gz archive with task description
   * @return result of the solution
   */
  [[nodiscard]] virtual sat::State solve(sat::Problem const& problem) = 0;

  /**
   * @brief Interrupts solving.
   */
  void interrupt();

  /**
   * @return The satisfying model if any has been found.
   */
  [[nodiscard]] virtual Mini::vec<Mini::lbool> get_model() const = 0;

 protected:
  [[nodiscard]] static sat::solver::RSolver _resolve_solver(
      SolverConfig const& config);

  [[nodiscard]] bool _is_interrupted() const noexcept;

  template <typename T>
  static void _interrupt(std::shared_ptr<T> shp) {
    if (shp != nullptr) {
      shp->interrupt();
    }
  }

 protected:
  virtual void _interrupt_impl() = 0;

 private:
  bool _interrupted = false;
};

MAKE_REFS(Solve);

DEFINE_REGISTRY(Solve, SolveConfig, solve);

}  // namespace core::solve

#endif  // ITMO_PARSAT_SOLVE_H
