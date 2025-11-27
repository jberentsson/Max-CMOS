/// @file       seidr.Quantizer.cpp
///	@ingroup 	seidr
///	@copyright	Copyright 2025 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License
///             found in the License.md file.

#include "seidr.Quantizer.hpp"

using namespace c74::min;

QuantizerMax::QuantizerMax(const atoms &args) {
    if (!args.empty()) {
        // TODO: Add some arguments.
    }
}

// Process note implementation
auto QuantizerMax::processNote(int note, int velocity) -> void { // NOLINT
    // Validate input.
    if ((note < MIDI::RANGE_LOW )|| (note > MIDI::RANGE_HIGH)) {
        return;
    }
    
    // Quantize the note.
    int quantizedNote = quantizer.quantize(note);
    
    // Send to outlets.
    output_note.send(quantizedNote);
    
    if (velocity <= MIDI::RANGE_HIGH) {
        output_velocity.send(velocity);
    }
}

MIN_EXTERNAL(QuantizerMax); // NOLINT
