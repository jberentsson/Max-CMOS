#include "seidr.NoteRandomOctave.hpp"

#include <iostream>

using namespace c74::min;

void NoteRandomOctaveMax::clearNoteMessage(int note) {
    int clearedCount = keyboard_.clearNotesByPitchClass(note);

    if (clearedCount > 0) {
        output0.send(note);
        output1.send(0);
    }
}

static void NoteRandomOctaveMax::clearAllNotesMessage() {
    // Send all notes off as fallback.
    for (int note = 0; note < 128; note++) {
        output0.send(note);
        output1.send(0);
    }
}

void NoteRandomOctaveMax::setRangeMessage(int low, int high) {
    keyboard_.setRandomRange(low, high);
}

void NoteRandomOctaveMax::printActiveNotes() {
    keyboard_.debugPrintActiveNotes();
}

NoteRandomOctaveMax::NoteRandomOctaveMax(const atoms &args) {
    // Nothing here.
}


void NoteRandomOctaveMax::processNoteMessage(int note, int velocity) {
    // Process the note.
    if (velocity > 0) {
        // Note ON
        const auto &activeNotes = keyboard_.getActiveNotes();

        if (activeNotes_.empty()) {
            return;
        }

        // Get the most recently added note.
        const auto &lastNote = activeNotes.back();

        // Send to outputs.
        output0.send(lastNote->pitch());
        output1.send(lastNote->velocity());
    } else {
        // Note OFF
        output0.send(note);
        output1.send(0);
    }
}

MIN_EXTERNAL(NoteRandomOctaveMax); // NOLINT
