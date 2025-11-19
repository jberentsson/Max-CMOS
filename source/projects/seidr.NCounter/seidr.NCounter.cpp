/// @file       seidr.NCounter.cpp
///	@ingroup 	jb
///	@copyright	Copyright 2022 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License
/// found in the License.md file.

#include "seidr.NCounter.hpp"

void NCounterMax::handleOutputs() {
    // Send data to the outputs.
    for (int i = 0; i < 10; i++) {
        this->outputs[i]->send(i == this->counter.value());
    }
}

unsigned int NCounterMax::counterValue() {
    return this->counter.value();
}

int NCounterMax::setPreset(int p) {
    return this->counter.setPreset(p);
}

int NCounterMax::preset() {
    return this->counter.preset();
}

int NCounterMax::step() {
    return this->counter.step();
}

MIN_EXTERNAL(NCounterMax);
