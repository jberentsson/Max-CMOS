/// @file       seidr.NCounter.cpp
///	@ingroup 	seidr
///	@copyright	Copyright 2022 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License
///             found in the License.md file.

#include "seidr.NCounter.hpp" // NOLINT
#include <iostream>

void NCounterMax::handleOutputs() {
    std::cout << "Counter: " << this->counter.value() << std::endl;
    for (int i = 0; i < NCounterMax::OUTPUT_COUNT; i++) {
        bool isActive = i == this->counter.value();
        this->outputs[i]->send(isActive);
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
