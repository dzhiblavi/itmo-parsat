#ifndef ITMO_PARSAT_SHAREDDATA_H
#define ITMO_PARSAT_SHAREDDATA_H

#include <memory>

#include "core/evol/instance/Fitness.h"
#include "core/domain/VarView.h"
#include "core/util/LRUCache.h"

namespace ea::instance {
/**
 * This struct is shared between all instances under the same algorithm.
 */
struct SharedData {
  /**
   * Sampling configuration.
   */
  struct SamplingConfig {
    uint32_t base_samples;
    uint32_t max_scale_steps;
    double scale;
  } sampling_config{};

  /**
   * Instances cache.
   */
  core::LRUCache<std::vector<bool>, Fitness> cache{};

  /**
   * Variable subset mapping.
   */
  core::domain::VarView var_view{};

  /**
   * Inaccurate number of points.
   * It is `inaccurate' because is calculated as number
   * of _calc_fitness calls, which happen on cache-misses.
   * Though, we limit the size of a cache, so this number
   * may be greater than the actual number of visited points.
   */
  uint32_t inaccurate_points = 0;

  /**
   * Inner configuration parameter.
   */
  uint32_t omega_x{};
};

using RSharedData = std::shared_ptr<SharedData>;

}  // namespace ea::instance

#endif  // ITMO_PARSAT_SHAREDDATA_H
