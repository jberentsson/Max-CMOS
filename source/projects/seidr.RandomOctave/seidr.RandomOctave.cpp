#include "seidr.RandomOctave.hpp"
#include "Utils/MIDI.hpp"

using namespace c74::min;

void RandomOctaveMax::clearNoteMessage(int note) {
    int clearedCount = randomOctave_.clearNotesByPitchClass(note);

    if (clearedCount > 0) {
        output_note.send(note);
        output_velocity.send(0);
    }
}

void RandomOctaveMax::clearAllNotesMessage() {
    // Send all notes off as fallback.
    for (int note = 0; note < MIDI::KEYBOARD_SIZE; note++) {
        output_note.send(note);
        output_velocity.send(0);
    }
}

void RandomOctaveMax::setRangeMessage(int low, int high) {
    randomOctave_.setRandomRange(low, high);
}

RandomOctaveMax::RandomOctaveMax(const atoms &args) {
    // Nothing here.
}

void RandomOctaveMax::processNoteMessage(int note, int velocity) { // NOLINT
    // Process the note.
    if (velocity > 0) {
        // Note ON
        const auto &activeNotes = randomOctave_.getActiveNotes();

        if (activeNotes.empty()) {
            return;
        }

        // Get the most recently added note.
        const auto &lastNote = activeNotes.back();

        // Send to outputs.
        output_note.send(lastNote->pitch());
        output_velocity.send(lastNote->velocity());
    } else {
        // Note OFF
        output_note.send(note);
        output_velocity.send(0);
    }
}

MIN_EXTERNAL(RandomOctaveMax); // NOLINT
