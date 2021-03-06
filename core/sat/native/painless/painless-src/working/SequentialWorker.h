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

#include "core/sat/native/painless/painless-src/solvers/SolverInterface.h"
#include "core/sat/native/painless/painless-src/utils/Threading.h"
#include "core/sat/native/painless/painless-src/working/WorkingStrategy.h"
#include "core/sat/native/mini/mtl/Vec.h"
#include "core/sat/native/mini/utils/Lit.h"

#include <optional>
#include <vector>
#include <mutex>
#include <thread>
#include <condition_variable>

namespace painless {

class SequentialWorker : public WorkingStrategy {
 public:
  SequentialWorker(WorkingResult* working_result, SolverInterface* solver_);

  ~SequentialWorker() noexcept override;

  void solve(int64_t index, Mini::vec<Mini::Lit> const& assumptions, vector<int> const& cube) override;

  void join(int64_t index, WorkingStrategy* winner, PSatResult res, Mini::vec<Mini::lbool> const& model) override;

  void setInterrupt() override;

  void waitInterrupt() override;

  void unsetInterrupt() override;

  int getDivisionVariable() override;

  void setPhase(const int var, const bool phase) override;

  void stop();

  void awaitStop() override;

  void bumpVariableActivity(const int var, const int times) override;

 protected:
  void main_worker_thread();

 private:
  bool _stop = false;
  std::atomic_bool _interrupted{false};
  int64_t current_index = 0;
  SolverInterface* solver = nullptr;

  std::mutex interrupt_mutex;
  std::condition_variable start_cv;
  std::mutex start_mutex;

  std::optional<std::tuple<int64_t, std::vector<int>, Mini::vec<Mini::Lit>>> _task;
  std::thread worker;
};

}  // namespace painless
