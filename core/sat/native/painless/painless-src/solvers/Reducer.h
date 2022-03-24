// -----------------------------------------------------------------------------
// Copyright (C) 2017  Ludovic LE FRIOUX
//
// This file is part of PaInleSS.
//
// PaInleSS is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// this program.  If not, see <http://www.gnu.org/licenses/>.
// -----------------------------------------------------------------------------

#pragma once

#include "../clauses/ClauseBuffer.h"
#include "../solvers/SolverInterface.h"
#include "../utils/Threading.h"

namespace painless {

/// Instance of a MapleCOMSPS solver
class Reducer : public SolverInterface {
 public:
  /// Load formula from a given dimacs file, return false if failed.
  bool loadFormula(const char* filename);

  bool loadFormula(std::vector<Mini::vec<Mini::Lit>> const& clauses);

  /// Get the number of variables of the current resolution.
  int getVariablesCount();

  /// Get a variable suitable for search splitting.
  int getDivisionVariable();

  /// Set initial phase for a given variable.
  void setPhase(const int var, const bool phase);

  /// Bump activity of a given variable.
  void bumpVariableActivity(const int var, const int times);

  /// Interrupt resolution, solving cannot continue until interrupt is unset.
  void setSolverInterrupt();

  /// Remove the SAT solving interrupt request.
  void unsetSolverInterrupt();

  PSatResult solve(Mini::vec<Mini::Lit> const& assumptions, const std::vector<int>& cube);

  /// Add a permanent clause to the formula.
  void addClause(ClauseExchange* clause);

  /// Add a list of permanent clauses to the formula.
  void addClauses(const std::vector<ClauseExchange*>& clauses);

  /// Add a list of initial clauses to the formula.
  void addInitialClauses(const std::vector<ClauseExchange*>& clauses);

  /// Add a learned clause to the formula.
  void addLearnedClause(ClauseExchange* clause);

  /// Add a list of learned clauses to the formula.
  void addLearnedClauses(const std::vector<ClauseExchange*>& clauses);

  /// Get a list of learned clauses.
  void getLearnedClauses(std::vector<ClauseExchange*>& clauses);

  /// Request the solver to produce more clauses.
  void increaseClauseProduction();

  /// Request the solver to produce less clauses.
  void decreaseClauseProduction();

  /// Get solver statistics.
  SolvingStatistics getStatistics();

  /// Return the model in case of SAT result.
  vector<int> getModel();

  /// Return the final analysis in case of UNSAT.
  vector<int> getFinalAnalysis();

  /// Native diversification.
  void diversify(int id);

  bool strengthed(ClauseExchange* cls, ClauseExchange** outCls);

  vector<int> getSatAssumptions();

  /// Constructor.
  Reducer(int id, SolverInterface* solver);

  /// Destructor.
  ~Reducer() noexcept;

 protected:
  /// Pointer to a MapleCOMSPS solver.
  SolverInterface* solver;

  /// Buffer used to import clauses (units included).
  ClauseBuffer clausesToImport;

  /// Buffer used to export clauses (units included).
  ClauseBuffer clausesToExport;

  Mini::vec<Mini::Lit> assumptions;

  bool _stop = false;
  bool _interrupt = false;
};

}  // namespace painless
