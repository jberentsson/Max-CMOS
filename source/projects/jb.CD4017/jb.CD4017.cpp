/// @file       jb.CD4017.cpp
///	@ingroup 	jb
///	@copyright	Copyright 2022 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License found in the License.md file.

#include "c74_min.h"
#include <catch.hpp>

#include "jb.CD4017.hpp"

void CD4017::send_output() {
    // Send data to the outputs.
    for (int i = 0; i < 10; i++){
        this->outputs[i]->send(i == this->counter);
    }
}

void CD4017::step() {
    this->counter++;

    if (this->counter >= 10) {
        this->reset_counter();
    }

    this->send_output();
}

void CD4017::reset_counter() {
    this->counter = -1;
}

MIN_EXTERNAL(CD4017);
