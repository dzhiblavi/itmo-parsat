#ifndef EVOL_PROP_H
#define EVOL_PROP_H

#include <functional>
#include <filesystem>
#include <memory>
#include <atomic>

#include "core/types.h"
#include "core/sat/SimpBase.h"
#include "core/search/Search.h"
#include "core/search/FullSearch.h"
#include "core/proto/solve_config.pb.h"
#include "core/sat/Problem.h"
#include "core/sat/SimpBase.h"
#include "util/Registry.h"
#include "util/mini.h"

namespace core::sat::prop {

/**
 * @brief SAT Propagation interface.
 */
class Prop {
 public:
  /**
   * @note Callbacks types must be thread safe for asynchronous
   * implementations.
   */
  // clang-format off
  typedef std::function<
      bool( // true iff should continue solving
        bool, // true iff there's been conflict
        lit_vec_t const& // assumptions passed to propagate
      )> prop_callback_t; //
  // clang-format on

 public:
  virtual ~Prop() = default;

  /**
   * @return native implementation
   */
  virtual std::vector<MinisatSimpBase*> native() noexcept = 0;

  /**
   * @brief Parses cnf from .gz file with the specified path.
   * @param path the path with formula.
   */
  virtual void load_problem(Problem const& problem) = 0;

  /**
   * @brief Returns the number of variables in formula.
   */
  [[nodiscard]] virtual uint32_t num_vars() const noexcept = 0;

  /**
   * @brief Propagates a given list of assumptions.
   * @return true iff conflicts happened.
   * @param assumptions the assumptions to include.
   * @param propagated the propagated literals.
   * @return true if and only if conflict occurred.
   */
  [[nodiscard]] virtual bool propagate(
      lit_vec_t const& assumptions, lit_vec_t& propagated) = 0;

  /**
   * @brief Propagates a given list of assumptions.
   * @param assumptions assumptions to include.
   * @return true if and only if conflict occurred.
   */
  [[nodiscard]] virtual bool propagate(lit_vec_t const& assumptions);

  /**
   * @brief Lightning fast version of full-search style propagate
   * @param vars variables for full search
   * @param head_size first head_size of vars will not be included in search
   * @return the number of conflicts in the subtree defined by vars, head_size
   */
  virtual uint64_t prop_tree(lit_vec_t const& vars, uint32_t head_size) = 0;

  /**
   * @brief Propagates all assignments by the given search specification and
   * calls callback respectively if it is present.
   * @param search search engine.
   * @param callback callback to be called on each solve.
   * @return the number of conflicts
   */
  virtual uint64_t prop_search(
      search::USearch search, prop_callback_t const& callback = {}) = 0;

  /**
   * @brief Propagates all assignments by the given search specification and
   * calls callback respectively if it is present.
   * @param base_assumption the independent assumption
   * @param search search engine.
   * @param callback callback to be called on each solve.
   * @return the number of conflicts
   */
  virtual uint64_t prop_search(
      lit_vec_t const& base_assumption, search::USearch search,
      prop_callback_t const& callback = {}) = 0;

  static uint64_t sequential_propagate(
      Minisat::SimpSolver& prop, search::USearch search,
      prop_callback_t const& callback);

  static uint64_t sequential_propagate(
      Minisat::SimpSolver& prop, lit_vec_t const& base_assumption,
      search::USearch search, prop_callback_t const& callback);
};

MAKE_REFS(Prop);

DEFINE_REGISTRY(Prop, PropConfig, prop);

}  // namespace core::sat::prop

#endif  // EVOL_PROP_H