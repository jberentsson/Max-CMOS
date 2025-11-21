/// @file       seidr.ShiftRegister.cpp
///	@ingroup 	seidr
///	@copyright	Copyright 2018 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License
/// found in the License.md file.

#include "c74_min.h"
#include "seidr.ShiftRegister.hpp"

#include <iostream>

using namespace c74::min;

ShiftRegisterMax::ShiftRegisterMax(const atoms &args) {
    int numberOfOutputs = 0;

    if (!args.empty()) {
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
        this->outputs[i]->send(this->sendBangs ? bang() : atoms({this->sr_.get(i)})); // NOLINT 
    }
}

void ShiftRegisterMax::handleThrough() {
    // Output N data through.
    int currentDataThrough = this->sr_.dataThrough();
    unsigned int lastOutputIndex = outputs.size() - 1;

    if (everyOutput || currentDataThrough != lastOutput[8].get()) { // NOLINT 
        this->outputs[lastOutputIndex]->send(this->sendBangs ? bang() : c74::min::atoms({currentDataThrough}));
        this->lastOutput[lastOutputIndex].set(currentDataThrough);
    }

    LastNote x = LastNote(); // NOLINT 
    x.set(currentDataThrough);

    this->lastOutput[8] = x; // NOLINT 
}

auto ShiftRegisterMax::size() -> int {
    return this->sr_.size();
}

auto ShiftRegisterMax::step() -> int {
    return this->sr_.step();
}

auto ShiftRegisterMax::get(int index) -> int {
    return this->sr_.get(index);
}

auto ShiftRegisterMax::dataInput(int value) -> int {
    return this->sr_.dataInput(value);
}

auto ShiftRegisterMax::dataThrough() ->  int {
    return this->sr_.dataThrough();
}

MIN_EXTERNAL(ShiftRegisterMax); // NOLINT
