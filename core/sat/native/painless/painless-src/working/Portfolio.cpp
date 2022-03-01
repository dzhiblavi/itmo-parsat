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

#include "../working/Portfolio.h"
#include "../working/SequentialWorker.h"

using namespace std;

namespace painless {

Portfolio::Portfolio(WorkingResult* working_result) : WorkingStrategy(working_result) {}

Portfolio::~Portfolio() {
  for (auto slave : slaves) {
    delete slave;
  }
}

void Portfolio::solve(const vector<int>& cube) {
  strategyEnding = false;

  for (size_t i = 0; i < slaves.size(); i++) {
    slaves[i]->solve(cube);
  }
}

void Portfolio::awaitStop() {
  for (size_t i = 0; i < slaves.size(); ++i) {
    slaves[i]->awaitStop();
  }
}

void Portfolio::join(WorkingStrategy* strat, PSatResult res, const vector<int>& model) {
  if (res == PUNKNOWN || strategyEnding || result->globalEnding)
    return;

  strategyEnding = true;

  setInterrupt();

  if (parent == NULL) {  // If it is the top strategy
    result->globalEnding = true;
    result->finalResult = res;

    if (res == PSAT) {
      result->finalModel = model;
    }
    SequentialWorker* winner = (SequentialWorker*) strat;
    // log(0, "The winner is thread %d \\o/ !!!\n", winner->solver->id);
  } else {  // Else forward the information to the parent strategy
    parent->join(this, res, model);
  }
}

void Portfolio::setInterrupt() {
  for (size_t i = 0; i < slaves.size(); i++) {
    slaves[i]->setInterrupt();
  }
}

void Portfolio::unsetInterrupt() {
  for (size_t i = 0; i < slaves.size(); i++) {
    slaves[i]->unsetInterrupt();
  }
}

void Portfolio::waitInterrupt() {
  for (size_t i = 0; i < slaves.size(); i++) {
    slaves[i]->waitInterrupt();
  }
}

int Portfolio::getDivisionVariable() {
  return 0;
}

void Portfolio::setPhase(int var, bool value) {}

void Portfolio::bumpVariableActivity(int var, int times) {}

}  // namespace painless