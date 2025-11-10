#pragma once
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <algorithm>

class ActiveNote {
private:
    int m_originalPitch;
    int m_processedPitch;
    int m_velocity;

public:
    ActiveNote(int originalPitch, int processedPitch, int velocity);
    int originalPitch() const;
    int pitch() const;
    int velocity() const;
};

class Keyboard {
private:
    static const int OCTAVE = 12;
    std::vector<std::unique_ptr<ActiveNote>> activeNotes;
    int rangeLow = 0;
    int rangeHigh = 127;
    int minOctave = 0;
    int maxOctave = 10;

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
    int Keyboard::maxCapacity() const {
        return 5;
    }
        
    int minCapacity() const {
        return 2; // Or whatever your minimum capacity should be
    }

    // Access to active notes (read-only)
    const std::vector<std::unique_ptr<ActiveNote>>& getActiveNotes() const;
    
    // Debug method
    void debugPrintActiveNotes() const;
};
