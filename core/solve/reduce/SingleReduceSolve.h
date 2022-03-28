#ifndef ITMO_PARSAT_SINGLEREDUCESOLVE_H
#define ITMO_PARSAT_SINGLEREDUCESOLVE_H

#include "core/proto/solve_config.pb.h"
#include "core/solve/reduce/ReduceSolve.h"
#include "core/solve/rbs/RBSearch.h"
#include "util/visit.h"

namespace core::solve {

/**
 * @brief This implementation simply solves problems generated by reduction.
 */
class SingleReduceSolve : public ReduceSolve {
 public:
  explicit SingleReduceSolve(SingleReduceSolveConfig config);

 protected:
  sat::State _solve_impl(
      ea::preprocess::RPreprocess const& preprocess) override;

  void _interrupt_impl() override;

 private:
  RRBSearch _rb_search;
  SingleReduceSolveConfig _cfg;
};

}  // namespace core::solve

#endif  // ITMO_PARSAT_SINGLEREDUCESOLVE_H