#include "seidr.RandomOctave.hpp"
#include "Utils/MIDI.hpp"

using namespace c74;

RandomOctaveMax::RandomOctaveMax(const min::atoms &args) {
    max::object_post((max::t_object*)this, "Constructor called with %d args", args.size());
    
    if (!args.empty() && args.size() >= 2) {
        int rangeLow = static_cast<int>(args[0]);
        int rangeHigh = static_cast<int>(args[1]);
        
        max::object_post((max::t_object*) this, "Setting range: %d to %d", rangeLow, rangeHigh);
        this->randomOctave_.setRange(rangeLow, rangeHigh);
    } else {
        max::object_post((max::t_object*) this, "Using default range");
    }
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
