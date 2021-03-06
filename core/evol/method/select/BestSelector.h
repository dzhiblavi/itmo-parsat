#ifndef EVOL_BESTSELECTOR_H
#define EVOL_BESTSELECTOR_H

#include "core/evol/method/select/Selector.h"

namespace ea::method {

/**
 * @brief Selects instances by their fitness.
 */
class BestSelector : public Selector {
 public:
  BestSelector() = default;

  void select(instance::Population& population, uint32_t size) override;
};

}  // namespace ea::method

#endif  // EVOL_BESTSELECTOR_H
