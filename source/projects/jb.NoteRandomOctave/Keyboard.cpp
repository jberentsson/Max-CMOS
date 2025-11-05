#include "Keyboard.hpp"
#include "Note.cpp"

Keyboard::Keyboard() {
    // Setup the notes on the keyboard.
    for (int i = 0; i < NOTES; i++) {
        this->notes.push_back(std::make_unique<Note>(i, &this->rangeLow, &this->rangeHigh));
    }
}

std::vector<ActiveNote*> Keyboard::note(int note, int velocity) {
    if (note >= 0 && note < this->notes.size()) {
        if(0 < velocity){
            return this->notes[note]->add(note);
        }
        return this->notes[note]->removeAll();
    }
    return {};  // Return empty vector if note out of range
}

int Keyboard::clearNotes(int n){
    if (n >= 0 && n <this-> notes.size()) {  // Check bounds
        return this->notes[n]->clearNotes();            
    }
    return -1;  // Error code      
}

int Keyboard::setRandomRange(int l, int h){
    // Set the range for the random octave.
    if (l > h) return -1;
    if (l < 0) return -2;
    if (h > 127) return -3;

    this->rangeLow = l;
    this->rangeHigh = h;
    
    return 0;
}
