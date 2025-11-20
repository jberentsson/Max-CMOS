/// @file       seidr.NCounter.cpp
///	@ingroup 	jb
///	@copyright	Copyright 2022 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License
/// found in the License.md file.

#include "seidr.NCounter.hpp"

void NCounterMax::handleOutputs() {
    // Send data to the outputs.
    for (int i = 0; i < 10; i++) {
        this->outputs[i]->send(i == this->counter_.value());
    }
}

unsigned int NCounterMax::counterValue() {
    return this->counter_.value();
}

unsigned int NCounterMax::setPreset(int p) {
    return this->counter_.setPreset(p);
}

unsigned  NCounterMax::preset() {
    return this->counter_.preset();
}

unsigned int NCounterMax::step() {
    return this->counter_.step();
}

MIN_EXTERNAL(NCounterMax); // NOLINT
