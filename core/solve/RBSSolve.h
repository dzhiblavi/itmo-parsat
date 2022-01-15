#ifndef ITMO_PARSAT_RBSSOLVE_H
#define ITMO_PARSAT_RBSSOLVE_H

#include <cmath>
#include <boost/timer/progress_display.hpp>

#include "core/evol/algorithm/Algorithm.h"
#include "core/solve/Solve.h"
#include "core/util/SigHandler.h"
#include "core/util/Tracer.h"
#include "core/util/Logger.h"
#include "core/domain/assignment/FullSearch.h"
#include "core/domain/assignment/RandomSearch.h"

namespace core {

/**
 * @brief This class solves SAT in the following way:
 * - Uses the specified (E|G)A to find rho-backdoor B;
 * - Solves SAT formula for each assignment of variables from B.
 */
class RBSSolve : public Solve {
 public:
  RBSSolve(RBSSolveConfig const& config);

  [[nodiscard]] sat::State solve(std::filesystem::path const& input) override;

 private:
  RBSSolveConfig _cfg;
};

}  // namespace core

#endif  // ITMO_PARSAT_RBSSOLVE_H