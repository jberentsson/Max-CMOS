/// @file       seidr.NCounter.cpp
///	@ingroup 	seidr
///	@copyright	Copyright 2022 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License
///             found in the License.md file.

#include "seidr.NCounter.hpp" // NOLINT

NCounterMax::NCounterMax(const atoms &args) {
    if (!args.empty()) {
        this->stepCount_ = args[0];
    }

    for (int i = 0; i < this->stepCount_; i++) {
        outputs.push_back(std::make_unique<outlet<>>(this, "(anything) output bit " + std::to_string(i)));
    }

    this->counter_ = Counter(this->stepCount_);
};

void NCounterMax::handleOutputs() {
    for (int i = 0; i < this->stepCount_; i++) {
        bool isActive = i == this->counter_.value();

        if (this->bangEnabled_ && isActive) {
            this->outputs[i]->send("bang");
        } else {
            this->outputs[i]->send(isActive);
        }
    }
}

auto NCounterMax::counterValue() -> unsigned int {
    return this->counter_.value();
}

MIN_EXTERNAL(NCounterMax); // NOLINT
