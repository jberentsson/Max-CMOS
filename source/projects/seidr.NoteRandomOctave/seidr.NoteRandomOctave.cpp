#include "seidr.NoteRandomOctave.hpp"
#include <iostream>

using namespace c74::min;

void NoteRandomOctave::clearNoteMessage(int note) {
    int clearedCount = keyboard.clearNotesByPitchClass(note);

    if (clearedCount > 0) {
        output_0.send(note);
        output_1.send(0);
    }
}

void NoteRandomOctave::clearAllNotesMessage() {
    // Send all notes off as fallback.
    for (int note = 0; note < 128; note++) {
        output_0.send(note);
        output_1.send(0);
    }
}

void NoteRandomOctave::setRangeMessage(int low, int high) {
    keyboard.setRandomRange(low, high);
}

void NoteRandomOctave::printActiveNotes() { keyboard.debugPrintActiveNotes(); }

NoteRandomOctave::NoteRandomOctave(const atoms &args) {
    // Nothing here.
}

NoteRandomOctave::~NoteRandomOctave() { clearAllNotesMessage(); }

void NoteRandomOctave::processNoteMessage(int note, int velocity) {
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
        output_0.send(lastNote->pitch());
        output_1.send(lastNote->velocity());
    } else {
        // Note OFF
        output_0.send(note);
        output_1.send(0);
    }
}

MIN_EXTERNAL(NoteRandomOctave);
