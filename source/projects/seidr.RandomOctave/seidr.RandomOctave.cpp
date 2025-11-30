#include "seidr.RandomOctave.hpp"
#include "Utils/MIDI.hpp"

using namespace c74::min;

RandomOctaveMax::RandomOctaveMax(const atoms &args) {
    // Nothing here.
}

auto RandomOctaveMax::clearNoteMessage(int note) -> void {
    //int clearedCount = this->randomOctave_.clear(note, 0);
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
    this->randomOctave_.setRange(low, high);
}

void RandomOctaveMax::processNoteMessage(int note, int velocity) { // NOLINT
    // Process the note.
    this->randomOctave_.note(note, velocity);

    for(const auto &currentNote : this->randomOctave_.getNoteQueue()) {
        // Send to outputs.
        if (currentNote->pitch() < MIDI::KEYBOARD_SIZE){
            output_note.send(currentNote->pitch());
            output_velocity.send(currentNote->velocity());
        }
    }

    this->randomOctave_.clearQueue();

}

MIN_EXTERNAL(RandomOctaveMax); // NOLINT
