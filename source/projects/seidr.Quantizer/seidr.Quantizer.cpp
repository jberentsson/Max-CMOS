/// @file       seidr.Quantizer.cpp
///	@ingroup 	seidr
///	@copyright	Copyright 2025 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License
///             found in the License.md file.

#include "seidr.Quantizer.hpp"

using namespace c74::min;

// Constructor implementation
QuantizerMax::QuantizerMax(const atoms &args) {
    //cout << "QuantizerMax created!!!" << endl;
    
    if (!args.empty()) {
        //cout << "With " << args.size() << " arguments" << endl;
        
        // Add initial notes if provided in constructor
        for (const auto& arg : args) {
            // SIMPLE: Direct cast without type checking
            int note = static_cast<int>(arg);
            if (note >= MIDI::RANGE_LOW && note <= MIDI::RANGE_HIGH) {
                this->quantizer.addNote(note);
                //cout << "Added initial note: " << note << endl;
            }
        }
    }
}

// Process note implementation
auto QuantizerMax::processNote(int note, int velocity) -> void {
    // Validate input
    if (note < MIDI::RANGE_LOW || note > MIDI::RANGE_HIGH) {
        //cout << "Note out of range: " << note << endl;
        return;
    }
    
    // Quantize the note
    int quantizedNote = quantizer.quantize(note);
    
    // Send to outlets
    output_note.send(quantizedNote);
    
    if (velocity <= MIDI::RANGE_HIGH) {
        output_velocity.send(velocity);
    }
    
    //cout << "Processed note: " << note << " -> " << quantizedNote << " (vel: " << velocity << ")\n" << endl;
}

MIN_EXTERNAL(QuantizerMax); // NOLINT
