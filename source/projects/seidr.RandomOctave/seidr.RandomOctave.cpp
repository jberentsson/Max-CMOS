#include "seidr.RandomOctave.hpp"
#include "Utils/MIDI.hpp"
#include <algorithm>

using namespace c74;

RandomOctaveMax::RandomOctaveMax(const min::atoms args) {
    // Default range
    int low = MIDI::RANGE_LOW;
    int high = MIDI::RANGE_HIGH;
    
    if (args.size() >= 2) {
        low = std::clamp(int(args[0]), MIDI::RANGE_LOW, MIDI::RANGE_HIGH);
        high = std::clamp(int(args[1]), MIDI::RANGE_LOW, MIDI::RANGE_HIGH);
        
        if (low > high) {
            std::swap(low, high);
        }
    }
    
    max::object_post((max::t_object*)this, "RandomOctaveMax initialized with range %d-%d", low, high);
    
    this->randomOctave_.setRange(low, high);
}

auto RandomOctaveMax::clearNoteMessage(int note) -> void {
    // Clear a single note.
    randomOctave_.note(note, 0);
    output_note.send(note);
    output_velocity.send(0);
    randomOctave_.clearQueue();
}

auto RandomOctaveMax::clearAllNotesMessage() -> void {
    // Send all notes off as fallback.
    this->randomOctave_.removeAll();

    for (int note = 0; note < MIDI::KEYBOARD_SIZE; note++) {
        output_note.send(note);
        output_velocity.send(0);
    }

    randomOctave_.clearQueue();
}

auto RandomOctaveMax::processNoteMessage(int note, int velocity) -> void { // NOLINT
    // The input needs to be an array with two integes.
    if(this->randomOctave_.note(note, velocity) == 0){ 
        for (const auto &currentNote : randomOctave_.getNoteQueue()) {
            // Send to outputs.
            output_note.send(currentNote->pitch());
            output_velocity.send(currentNote->velocity());
        }

        randomOctave_.clearQueue();
    }
}

MIN_EXTERNAL(RandomOctaveMax); // NOLINT
