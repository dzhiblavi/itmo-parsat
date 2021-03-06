#ifndef ITMO_PARSAT_UNIQUESEARCH_H
#define ITMO_PARSAT_UNIQUESEARCH_H

#include "unordered_set"

#include "core/search/Search.h"
#include "util/Random.h"

namespace core::search {

/**
 * @brief The class used to perform unique random search. Used for small sets of
 * variables.
 */
class UniqueSearch : public Search {
 public:
  explicit UniqueSearch(
      domain::VarView const& var_view, bit_mask_t const& vars, uint64_t total);

  explicit UniqueSearch(std::vector<int> const& vars, uint64_t total);

 private:
  std::unordered_set<uint64_t> visited_;

 protected:
  void _advance() override;

  void _reset() override;

  [[nodiscard]] UniqueSearch* clone() const override;

  void _advance_us();
};

MAKE_REFS(UniqueSearch);

}  // namespace core::search

#endif  // ITMO_PARSAT_UNIQUESEARCH_H
