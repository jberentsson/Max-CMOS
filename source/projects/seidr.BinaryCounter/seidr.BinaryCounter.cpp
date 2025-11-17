/// @file       seidr.BinaryCounter.cpp
///	@ingroup 	jb
///	@copyright	Copyright 2022 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License
/// found in the License.md file.

#include "seidr.BinaryCounter.hpp"

BinaryCounter_MAX::BinaryCounter_MAX(const atoms &args) {
    for (int i = 0; i < args.size(); i++) {
        cout << "arg[" << i << "]" << args[i] << endl;
    }

    for (int i = 0; i < 8; i++) {
        outputs.push_back(std::make_unique<outlet<>>(
            this, "(anything) output bit " + std::to_string(i)));
    }
}

int BinaryCounter_MAX::get_bit(int output) {
    // Isolate the correct bit.
    return ((this->counter.value()) >> output) & 0x1;
}

void BinaryCounter_MAX::update_outputs() {
    // Send data to the outputs.
    for (int i = 0; i < OUTPUT_COUNT; i++) {
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

void BinaryCounter_MAX::enable_bangs() { this->bang_enabled = TRUE; }

void BinaryCounter_MAX::disable_bangs() { this->bang_enabled = FALSE; }

int BinaryCounter_MAX::counter_value() { return this->counter.value(); }

int BinaryCounter_MAX::set_preset(int p) { return this->counter.set_preset(p); }

int BinaryCounter_MAX::preset() { return this->counter.preset(); }

int BinaryCounter_MAX::max_value() { return this->counter.get_max_value(); }

MIN_EXTERNAL(BinaryCounter_MAX);
