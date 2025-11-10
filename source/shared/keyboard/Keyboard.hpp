#pragma once
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include "Note.hpp"

#define MIN_CAPACITY 2
#define MAX_CAPACITY 5
#define RANGE_LOW    0
#define RANGE_HIGH   127
#define MIN_OCTAVE   0
#define MAX_OCTAVE   10

class Keyboard {
private:
    static const int OCTAVE = 12;
    std::vector<std::unique_ptr<ActiveNote>> activeNotes;
    int rangeLow = RANGE_LOW;
    int rangeHigh = RANGE_HIGH;
    int minOctave = MIN_OCTAVE;
    int maxOctave = MAX_OCTAVE;

    int getPitchClass(int pitch) const;
    int clampPitchToRange(int pitch);
    int randomizeNote(int pitch);

public:
    Keyboard(int low = 0, int high = 127);
    
    // Main note processing method - returns number of notes processed
    int note(int pitch, int velocity);
    
    // Note clearing methods - return number of notes cleared
    int clearNotesByPitchClass(int pitch);
    int removeAll();
    
    // Range configuration
    void updateRange(int low, int high);
    void setRandomRange(int low, int high);
    
    // Getters
    int maxCapacity() const {
        return MAX_CAPACITY;
    }
        
    int minCapacity() const {
        return MIN_CAPACITY; // Or whatever your minimum capacity should be
    }

    // Access to active notes (read-only)
    const std::vector<std::unique_ptr<ActiveNote>>& getActiveNotes() const;
    
    // Debug method
    void debugPrintActiveNotes() const;
};
