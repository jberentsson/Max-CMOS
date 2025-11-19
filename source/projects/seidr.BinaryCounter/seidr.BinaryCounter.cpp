/// @file       seidr.BinaryCounter.cpp
///	@ingroup 	jb
///	@copyright	Copyright 2022 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License
/// found in the License.md file.

#include "seidr.BinaryCounter.hpp"

BinaryCounterMax::BinaryCounterMax(const atoms &args) {
    // Create outputs
    for (int i = 0; i < OUTPUT_COUNT; i++) {
        outputs.push_back(
            std::make_unique<outlet<>>(this, "(anything) output bit " + std::to_string(i)));
    }
    
    updateOutputs();
}

unsigned int BinaryCounterMax::getBit(int output) {
    return ((this->counter_.value()) >> output) & 0x1;
}

void BinaryCounterMax::updateOutputs() {
    // Send data to all outputs
    for (int i = 0; i < OUTPUT_COUNT; i++) {
        int current = OUTPUT_COUNT - i - 1;
        
        if (this->bang_enable) {
            if (this->getBit(i) == 1) {
                this->outputs[current]->send("bang");
            }
        } else {
            this->outputs[current]->send(this->getBit(i));
        }
    }
}

void BinaryCounterMax::enableBangs() {
    this->bang_enable = true;
    this->updateOutputs();
}

void BinaryCounterMax::disableBangs() {
    this->bang_enable = false;
    this->updateOutputs();
}

unsigned int BinaryCounterMax::counterValue() {
    return this->counter_.value();
}

unsigned int BinaryCounterMax::setPreset(unsigned int p) {
    unsigned int result = this->counter_.setPreset(p);
    this->updateOutputs();
    return result;
}

unsigned int BinaryCounterMax::preset() {
    return this->counter_.preset();
}

unsigned int BinaryCounterMax::maxValue() {
    return this->counter_.getMaxValue();
}

MIN_EXTERNAL(BinaryCounterMax);
