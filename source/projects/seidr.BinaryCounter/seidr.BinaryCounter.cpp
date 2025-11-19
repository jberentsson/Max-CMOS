/// @file       seidr.BinaryCounter.cpp
///	@ingroup 	jb
///	@copyright	Copyright 2022 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License
/// found in the License.md file.

#include "seidr.BinaryCounter.hpp"

BinaryCounterMax::BinaryCounterMax(const atoms &args) {
    for (int i = 0; i < args.size(); i++) {
        cout << "arg[" << i << "]" << args[i] << endl;
    }

    for (int i = 0; i < 8; i++) {
        outputs.push_back(
            std::make_unique<outlet<>>(this, "(anything) output bit " + std::to_string(i)));
    }
}

unsigned int BinaryCounterMax::getBit(int output) {
    // Isolate the correct bit.
    return ((this->counter.value()) >> output) & 0x1;
}

void BinaryCounterMax::updateOutputs() {
    // Send data to the outputs.
    for (int i = 0; i < OUTPUT_COUNT; i++) {
        int current = OUTPUT_COUNT - i - 1;

        if (this->bangEnable) {
            if (this->getBit(i) == 1) {
                this->outputs[current]->send("bang");
            }
        } else {
            this->outputs[current]->send(this->getBit(i));
        }
    }
}

void BinaryCounterMax::enableBangs() {
    this->bangEnable = TRUE;
}

void BinaryCounterMax::disableBangs() {
    this->bangEnable = FALSE;
}

unsigned int BinaryCounterMax::counterValue() {
    return this->counter.value();
}

int BinaryCounterMax::setPreset(int p) {
    return this->counter.setPreset(p);
}

int BinaryCounterMax::preset() {
    return this->counter.preset();
}

int BinaryCounterMax::maxValue() {
    return this->counter.getMaxValue();
}

MIN_EXTERNAL(BinaryCounterMax);
