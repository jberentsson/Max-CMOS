#include "seidr.RandomOctave.hpp"
#include "Utils/MIDI.hpp"

using namespace c74::min;

RandomOctaveMax::RandomOctaveMax(const atoms &args) {
    // Nothing here.
}

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

auto RandomOctaveMax::setRangeMessage(int low, int high) -> void {
    randomOctave_.setRange(low, high);
}

void RandomOctaveMax::processNoteMessage(int note, int velocity) { // NOLINT
    // Process the note.
    this->randomOctave_.note(note, velocity);

    for(const auto &currentNote : this->randomOctave_.getQueuedNotes()) {
        // Send to outputs.
        if (currentNote->pitch() < MIDI::KEYBOARD_SIZE){
            output_note.send(currentNote->pitch());
            output_velocity.send(currentNote->velocity());
        }
    }

    this->randomOctave_.clearQueue();

}

MIN_EXTERNAL(RandomOctaveMax); // NOLINT
