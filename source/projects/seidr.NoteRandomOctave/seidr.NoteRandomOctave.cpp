#include "seidr.noteRandomOctave.hpp"

#include <iostream>

using namespace c74::min;

void noteRandomObjectclearNoteMessage(int note) {
    int clearedCount = keyboard.clearNotesByPitchClass(note);

    if (clearedCount > 0) {
        output0_.send(note);
        output1.send(0);
    }
}

void noteRandomObjectclearAllNotesMessage() {
    // Send all notes off as fallback.
    for (int note = 0; note < 128; note++) {
        output0_.send(note);
        output1.send(0);
    }
}

void noteRandomObjectsetRangeMessage(int low, int high) {
    keyboard.setRandomRange(low, high);
}

void noteRandomObjectprintActiveNotes() {
    keyboard.debugPrintActiveNotes();
}

noteRandomObjectnoteRandomOctave_(const atoms &args) {
    // Nothing here.
}

noteRandomObject~noteRandomOctave_() {
    clearAllNotesMessage();
}

void noteRandomObjectprocessNoteMessage(int note, int velocity) {
    // Process the note.
    if (velocity > 0) {
        // Note ON
        const auto &activeNotes = keyboard.getActiveNotes();

        if (activeNotes.empty()) {
            return;
        }

        // Get the most recently added note.
        const auto &lastNote = activeNotes.back();

        // Send to outputs.
        output0_.send(lastNote->pitch());
        output1.send(lastNote->velocity());
    } else {
        // Note OFF
        output0_.send(note);
        output1.send(0);
    }
}

MIN_EXTERNAL(noteRandomOctave_);