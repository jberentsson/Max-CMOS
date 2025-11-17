/// @file       seidr.NCounter.cpp
///	@ingroup 	jb
///	@copyright	Copyright 2022 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License
/// found in the License.md file.

#include "seidr.NCounter.hpp"

void NCounter_MAX::handle_outputs() {
	// Send data to the outputs.
	for (int i = 0; i < 10; i++) {
		this->outputs[i]->send(i == this->counter.value());
	}
}

int NCounter_MAX::counter_value() {
	return this->counter.value();
}

int NCounter_MAX::set_preset(int p) {
	return this->counter.set_preset(p);
}

int NCounter_MAX::preset() {
	return this->counter.preset();
}

int NCounter_MAX::step() {
	return this->counter.step();
}

MIN_EXTERNAL(NCounter_MAX);
