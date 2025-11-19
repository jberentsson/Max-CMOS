/// @file       seidr.ShiftRegister.cpp
///	@ingroup 	jb
///	@copyright	Copyright 2018 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License
/// found in the License.md file.

#include "seidr.ShiftRegister.hpp"

#include "c74_min.h"

#include <iostream>

using namespace c74::min;

ShiftRegisterMax::ShiftRegisterMax(const atoms &args) {
    int numberOfOutputs = 0;

    if (args.size() > 0) {
        numberOfOutputs = args[0];
    } else {
        numberOfOutputs = OUTPUT_COUNT;
    }

    for (int i = 0; i < numberOfOutputs; i++) {
        outputs.push_back(
            std::make_unique<outlet<>>(this, "( int | bang ) output " + std::to_string(i)));
    }

    for (int i = 0; i < args.size(); i++) {
        cout << "arg[" << i << "]" << args[i] << endl;
    }
};

void ShiftRegisterMax::handleOutputs() {
    // Bit outputs from 0 to (N-1).
    for (int i = 0; i < outputs.size() - 1; i++) {
        this->outputs[i]->send(this->sendBangs ? bang() : atoms({(uint64_t)this->sr_.get(i)}));
    }
}

void ShiftRegisterMax::handleThrough() {
    // Output N data through.
    uint64_t dt = this->sr_.dataThrough();
    int lastOutputIndex = outputs.size() - 1;

    if (everyOutput || dt != lastOutput[8].get()) {
        this->outputs[lastOutputIndex]->send(this->sendBangs ? bang() : c74::min::atoms({dt}));
        this->lastOutput[lastOutputIndex].set(dt);
    }

    LastNote x = LastNote();
    x.set(dt);

    this->lastOutput[8] = x;
}

int ShiftRegisterMax::size() {
    return this->sr_.size();
}

int ShiftRegisterMax::step() {
    return this->sr_.step();
}

uint64_t ShiftRegisterMax::get(int i) {
    return this->sr_.get(i);
}

int ShiftRegisterMax::dataInput(int v) {
    return this->sr_.dataInput(v);
}

int ShiftRegisterMax::dataThrough() {
    return this->sr_.dataThrough();
}

MIN_EXTERNAL(ShiftRegisterMax);
