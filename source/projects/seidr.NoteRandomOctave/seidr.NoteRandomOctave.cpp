#include "seidr.NoteRandomOctave.hpp"
#include "Utils/MIDI.hpp"

using namespace c74::min;

void NoteRandomOctaveMax::clearNoteMessage(int note) {
    int clearedCount = keyboard_.clearNotesByPitchClass(note);

    if (clearedCount > 0) {
        output_note.send(note);
        output_velocity.send(0);
    }
}

void NoteRandomOctaveMax::clearAllNotesMessage() {
    // Send all notes off as fallback.
    for (int note = 0; note < MIDI::KEYBOARD_SIZE; note++) {
        output_note.send(note);
        output_velocity.send(0);
    }
}

void NoteRandomOctaveMax::setRangeMessage(int low, int high) {
    keyboard_.setRandomRange(low, high);
}

NoteRandomOctaveMax::NoteRandomOctaveMax(const atoms &args) {
    // Nothing here.
}

void NoteRandomOctaveMax::processNoteMessage(int note, int velocity) { // NOLINT
    // Process the note.
    if (velocity > 0) {
        // Note ON
        const auto &activeNotes = keyboard_.getActiveNotes();

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

MIN_EXTERNAL(NoteRandomOctaveMax); // NOLINT
