#ifndef ITMO_PARSAT_CARTESIANSEARCH_H
#define ITMO_PARSAT_CARTESIANSEARCH_H

#include <vector>
#include <numeric>
#include <algorithm>

#include "core/domain/assignment/Search.h"
#include "core/util/Logger.h"

namespace core::domain {

class CartesianSearch;
MAKE_REFS(CartesianSearch);

/**
 * @brief The class used to search through the cartesian product of custom searches.
 */
class CartesianSearch : public Search {
 public:
  /**
   * @param assignments the set of assignments to be searched through.
   */
  explicit CartesianSearch(std::vector<std::vector<std::vector<Minisat::Lit>>>&& cartesian);

  /**
   * @see Search::operator()()
   */
  Minisat::vec<Minisat::Lit> const& operator()() const override;

 protected:
  void _advance() override;

  void _set_cur(uint32_t from);

 private:
  std::vector<uint32_t> _indices;
  Minisat::vec<Minisat::Lit> _assignment;
  std::vector<std::vector<std::vector<Minisat::Lit>>> _cartesian;
};

UCartesianSearch createCartesianSearch(
    std::vector<std::vector<std::vector<Minisat::Lit>>>&& cartesian);

}  // namespace core::domain

#endif  // ITMO_PARSAT_CARTESIANSEARCH_H
