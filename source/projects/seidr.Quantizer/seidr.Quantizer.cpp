#include <c74_min.h>
#include "seidr.Quantizer.hpp"

using namespace c74;

QuantizerMax::QuantizerMax(const min::atoms &args) {
    if (!args.empty()) {
        // QuantizeMode
        if (!args.empty()) {
            this->quantizer_.setMode(Quantizer::QuantizeMode(static_cast<int>(args[0])));
        }

        // RoundDirection
        if (args.size() >= 2) {
            this->quantizer_.setRoundDirection(Quantizer::RoundDirection(static_cast<int>(args[1])));
        }

        // Range
        if (args.size() == 4) { 
            uint8_t rangeLow = static_cast<int>(args[2]);
            uint8_t rangeHigh = static_cast<int>(args[3]);
            this->quantizer_.setRange(Quantizer::Note(rangeLow), Quantizer::Note(rangeHigh));
        }
    }
}

auto QuantizerMax::processNoteMessage(int notePitch, int velocity) -> void { // NOLINT
    // Validate input.
    if ((notePitch < MIDI::RANGE_LOW) || (notePitch > MIDI::RANGE_HIGH)) {
        return;
    }
    
    // Quantize the note.
    int quantizedNote = this->quantizer_.quantize(MIDI::Note(notePitch));
    
    // Send to outlets.
    output_note.send(quantizedNote);
    
    if (velocity <= MIDI::RANGE_HIGH) {
        output_velocity.send(velocity);
    }
}

// The GitHub compiler was complaining this file was missing.
MIN_EXTERNAL(QuantizerMax); // NOLINT
