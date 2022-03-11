#ifndef ITMO_PARSAT_ASSIGNMENTMODIFIER_H
#define ITMO_PARSAT_ASSIGNMENTMODIFIER_H

#include <cmath>
#include <algorithm>

#include "core/domain/assignment/Search.h"

namespace core::domain {

/**
 * @brief The base class for searches that step by modifying assignment.
 */
class AssignmentModifier {
 public:
  AssignmentModifier(VarView const& var_view, std::vector<bool> const& vars);

  AssignmentModifier(std::vector<int> const& vars);

  [[nodiscard]] Mini::vec<Mini::Lit> const& get() const;

 protected:
  Mini::vec<Mini::Lit> _assignment;
};

}  // namespace core::domain

#endif  // ITMO_PARSAT_ASSIGNMENTMODIFIER_H
