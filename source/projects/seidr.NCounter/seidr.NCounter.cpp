/// @file       seidr.NCounter.cpp
///	@ingroup 	seidr
///	@copyright	Copyright 2022 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License
///             found in the License.md file.

#include "seidr.NCounter.hpp" // NOLINT

void NCounterMax::handleOutputs() {
    // Send data to the outputs.
    for (int i = 0; i < NCounterMax::OUTPUT_COUNT; i++) {
        this->outputs[i]->send(i == this->counter.value());
    }
}

auto NCounterMax::counterValue() -> unsigned int {
    return this->counter.value();
}

auto NCounterMax::setPreset(int presetValue) -> unsigned int {
    return this->counter.setPreset(presetValue);
}

auto NCounterMax::preset() -> unsigned int {
    return this->counter.preset();
}

auto NCounterMax::step() -> unsigned int {
    return this->counter.step();
}

MIN_EXTERNAL(NCounterMax); // NOLINT
