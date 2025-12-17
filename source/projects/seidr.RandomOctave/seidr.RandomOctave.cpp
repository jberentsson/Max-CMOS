#include "seidr.RandomOctave.hpp"
#include "Utils/MIDI.hpp"
#include <algorithm>

using namespace c74;
using NoteReturnCodes = MIDI::NoteReturnCodes;

RandomOctaveMax::RandomOctaveMax(const min::atoms &args) {
    // Default range
    int low = MIDI::RANGE_LOW;
    int high = MIDI::RANGE_HIGH;
    
    if (args.size() >= 2) {
        low = std::clamp(static_cast<int> (args[0]), MIDI::RANGE_LOW, MIDI::RANGE_HIGH);
        high = std::clamp(static_cast<int> (args[1]), MIDI::RANGE_LOW, MIDI::RANGE_HIGH);
        
        if (low > high) {
            std::swap(low, high);
        }
    }
    
    this->randomOctave_.setRange(low, high);
}

auto RandomOctaveMax::clearNoteMessage(int note) -> void {
    // Clear a single note.
    randomOctave_.note(note, 0);
    output_velocity.send(0);
    output_note.send(note);
    randomOctave_.clearQueue();
}

auto RandomOctaveMax::clearAllNotesMessage() -> void {
    // Send all notes off as fallback.
    this->randomOctave_.removeAll();

    for (int note = 0; note < MIDI::KEYBOARD_SIZE; note++) {
        output_velocity.send(0);
        output_note.send(note);
    }

    randomOctave_.clearQueue();
}

auto RandomOctaveMax::processNoteMessage(int note, int velocity) -> void { // NOLINT
    // The input needs to be an array with two integes.
    if (this->randomOctave_.note(note, velocity) == NoteReturnCodes::OK) { 
        for (const auto &currentNote : randomOctave_.getNoteQueue()) {
            // Send to outputs.
            output_velocity.send(currentNote->velocity());
            output_note.send(currentNote->pitch());
        }

        randomOctave_.clearQueue();
    }
}

MIN_EXTERNAL(RandomOctaveMax); // NOLINT
