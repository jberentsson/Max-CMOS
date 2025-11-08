/// @file       jb.CD4024.cpp
///	@ingroup 	jb
///	@copyright	Copyright 2018 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License found in the License.md file.

#include "jb.CD4024.hpp"

using namespace c74::min;

int CD4024::find_bit(int output) {
    // isolate the correct bit.
    return ((this->counter) >> output) & 0x1;
}

void CD4024::handle_output() {
    // Send data to the outputs
    // TODO: Send only data on change

    if (this->bang_enabled) {
        this->send_bangs();
    } else {
        for (int i = 0; i < 7; i++){
            this->outputs[i]->send(this->find_bit(i));
        }
    }
}

void CD4024::send_bangs() {
    for (int i = 0; i < 7; i++){
        if (this->find_bit(i)) {
            this->outputs[i]->send("bang");
        }
    }
}

void CD4024::step() {
    this->counter++;
    this->handle_output();
}

void CD4024::reset_counter() {
    this->counter = 0;
}

MIN_EXTERNAL(CD4024);
