#include "Note.hpp"
#include <algorithm>

Note::Note(int p, int* low, int* high) : notePitch(p), rangeLow(*low), rangeHigh(*high) {
    maxCapacity();
    minCapacity();
}

int Note::maxCapacity(){
    this->maxOctave = this->rangeHigh / OCTAVE;
    
    if (this->rangeHigh % OCTAVE != 0) {
        this->maxOctave++;
    }

    return this->maxOctave;
}

int Note::minCapacity(){
    this->minOctave = (this->rangeLow - (this->rangeLow % OCTAVE)) / OCTAVE;

    if (this->minOctave > this->maxOctave) {
        this->minOctave = this->maxOctave = this->notePitch / OCTAVE;
    }

    return this->minOctave;
}

int Note::randomizeNote(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(this->minOctave, this->maxOctave);
    int randomOctave = dist(gen);

    int noteClass = this->notePitch % OCTAVE;
    int result = noteClass + randomOctave * OCTAVE;

    // Check if result is within range, if not find nearest valid octave
    if (result < this->rangeLow || result > this->rangeHigh) {
        // Find the closest valid octave
        int basePitch = noteClass;
        int lowOctave = (this->rangeLow - basePitch) / OCTAVE;
        int highOctave = (this->rangeHigh - basePitch) / OCTAVE;
        
        // Clamp the octave to valid range
        randomOctave = std::clamp(randomOctave, lowOctave, highOctave);
        result = basePitch + randomOctave * OCTAVE;
    }

    // Final safety check
    result = std::clamp(result, this->rangeLow, this->rangeHigh);
    
    return result;
}

std::vector<ActiveNote*> Note::add(int n) {
    std::vector<ActiveNote*> notes;

    if (activeNotes.size() < NOTE_CAPACITY) {
        int newNote = randomizeNote();
        auto newNotePtr = std::make_unique<ActiveNote>(newNote);
        
        // Store the unique_ptr
        activeNotes.push_back(std::move(newNotePtr));
        
        // Return raw pointer to the stored object
        notes.push_back(activeNotes.back().get());
        this->count++;
    }

    return notes;
}

std::vector<ActiveNote*> Note::removeAll() {
    std::vector<ActiveNote*> notes;

    for (size_t i = 0; i < activeNotes.size(); i++) {
        if (activeNotes[i] != nullptr) {
            // Get raw pointer without transferring ownership
            notes.push_back(activeNotes[i].get());
        }
    }
    
    // Clear the activeNotes vector (this will delete the objects)
    activeNotes.clear();
    this->count = 0;
    
    return notes;
}

int Note::clearNotes() {
    activeNotes.clear();
    this->count = 0;
    return 0;
}

std::vector<std::unique_ptr<ActiveNote>>& Note::active() {
    return this->activeNotes;
}

std::vector<ActiveNote*> Note::getActiveNotes() {
    std::vector<ActiveNote*> notes;
    for (auto& note : activeNotes) {
        if (note != nullptr) {
            notes.push_back(note.get());
        }
    }
    return notes;
}
