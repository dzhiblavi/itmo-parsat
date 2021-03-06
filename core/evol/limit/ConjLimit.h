#ifndef EVOL_CONJLIMIT_H
#define EVOL_CONJLIMIT_H

#include <memory>

#include "core/evol/limit/Limit.h"

namespace ea::limit {

/**
 * @brief Conjunction of a set of limits.
 */
class ConjLimit : public Limit {
 public:
  explicit ConjLimit(const ConjLimitConfig& config);

  void start() override;

 protected:
  /**
   * @return if *all* of the limits returned true.
   */
  bool _proceed(ea::algorithm::Algorithm& algorithm) override;

 private:
  std::vector<RLimit> _limits;
};

}  // namespace ea::limit

#endif  // EVOL_CONJLIMIT_H
