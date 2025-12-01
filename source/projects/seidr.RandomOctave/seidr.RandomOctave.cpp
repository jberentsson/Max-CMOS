#include "seidr.RandomOctave.hpp"
#include "Utils/MIDI.hpp"
#include <iostream>

using namespace c74::min;

auto RandomOctaveMax::clearNoteMessage(int note) -> void{
    //int clearedCount = randomOctave_.clearNotesByPitchClass(note);
    //
    //if (clearedCount > 0) {
    //    output_note.send(note);
    //    output_velocity.send(0);
    //}
}

auto RandomOctaveMax::clearAllNotesMessage() -> void {
    // Send all notes off as fallback.
    for (int note = 0; note < MIDI::KEYBOARD_SIZE; note++) {
        output_note.send(note);
        output_velocity.send(0);
    }
}

auto RandomOctaveMax::setRangeMessage(int low, int high) -> void {
    randomOctave_.setRange(low, high);
}

RandomOctaveMax::RandomOctaveMax(const atoms &args) {
    // Nothing here.
}

auto RandomOctaveMax::processNoteMessage(atoms args) -> void { // NOLINT
    // The input needs to be an array with two integes.
    if (args.size() >= 2) {
        int note = args[0];
        int velocity = args[1];

        if(this->randomOctave_.note(note, velocity) == 0){ 
            if (randomOctave_.getNoteQueue().empty()) {
                return;
            }

            for (const auto &currentNote : randomOctave_.getNoteQueue()) {
                // Send to outputs.
                output_note.send(currentNote->pitch());
                output_velocity.send(currentNote->velocity());
            }

            randomOctave_.clearQueue();
        }
    }
}

MIN_EXTERNAL(RandomOctaveMax); // NOLINT
