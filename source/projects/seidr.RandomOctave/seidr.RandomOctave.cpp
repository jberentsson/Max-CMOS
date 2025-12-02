#include "seidr.RandomOctave.hpp"
#include "Utils/MIDI.hpp"

using namespace c74::min;

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

auto RandomOctaveMax::setRangeMessage(int low, int high) -> void {
    randomOctave_.setRange(low, high);
}

RandomOctaveMax::RandomOctaveMax(const c74::min::atoms &args) {
    // Nothing here.
}

auto RandomOctaveMax::processNoteMessage(atoms args) -> void { // NOLINT
    // The input needs to be an array with two integes.
    if (args.size() >= 2) {
        int note = args[0];
        int velocity = args[1];

        if(this->randomOctave_.note(note, velocity) == 0){ 
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
