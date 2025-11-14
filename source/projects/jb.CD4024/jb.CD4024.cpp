/// @file       jb.CD4024.cpp
///	@ingroup 	jb
///	@copyright	Copyright 2018 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License found in the License.md file.

#include "jb.CD4024.hpp"

using namespace c74::min;

int CD4024_MAX::get_bit(int output) {
    // isolate the correct bit.
    return ((this->counter.value()) >> output) & 0x1;
}

void CD4024_MAX::update_outputs() {
    // Send data to the outputs
    for (int i = 0; i < OUTPUT_COUNT; i++){
        int current = OUTPUT_COUNT - i - 1;

        if (this->bang_enabled) {
            if (this->get_bit(i)) {
                this->outputs[current]->send("bang");
            }
        } else {
            this->outputs[current]->send(this->get_bit(i));
        }
    }
}

void CD4024_MAX::enable_bangs() {
    this->bang_enabled = TRUE;
}

void CD4024_MAX::disable_bangs() {
    this->bang_enabled = FALSE;
}

int CD4024_MAX::counter_value() {
    return this->counter.value();
}

int CD4024_MAX::set_preset(int p){
    return this->counter.set_preset(p);
}

int CD4024_MAX::preset(){
    return this->counter.preset();
}

int CD4024_MAX::max_value(){
    return this->counter.get_max_value();
}

MIN_EXTERNAL(CD4024_MAX);
