#ifndef EVOL_RESOURCELIMIT_H
#define EVOL_RESOURCELIMIT_H

#include "core/evol/limit/Limit.h"
#include "core/util/mem_usage.h"
#include "core/util/Tracer.h"

namespace ea::limit {

/**
 * @brief This limit controls the execution by limiting time and memory consumption of an algorithm.
 */
class ResourceLimit : public Limit {
 public:
  ResourceLimit(ResourceLimitConfig const& config);

  void start() override;

 protected:
  /**
   * @return true iff time and memory limits are both not reached and there
   *         still are unvisited points left.
   */
  bool _proceed(ea::algorithm::Algorithm& algorithm) override;

 private:
  uint32_t _memory_limit_kb;
  uint32_t _time_limit_sec;
  std::chrono::time_point<core::clock_t> _start;
};

}  // namespace ea::limit

#endif  // EVOL_RESOURCELIMIT_H
