#include "seidr.Quantizer.hpp"
#include "Utils/MIDI.hpp"

using namespace c74::min;

QuantizerMax::QuantizerMax(const atoms &args) {
    
}

auto QuantizerMax::processNote(int note, int velocity) -> void{ // NOLINT
    int quantizedNote = quantizer.quantize(note);

    if (quantizer.noteCount() > 0) {
        if (velocity >= MIDI::RANGE_HIGH + 1) {
            output0.send(quantizedNote);
        } else if (velocity <= MIDI::RANGE_HIGH) {
            output0.send(quantizedNote);
            output1.send(velocity);
        }
    }
}

MIN_EXTERNAL(QuantizerMax); // NOLINT
