#include <iostream>
#include "Keyboard.hpp"

ActiveNote::ActiveNote(int originalPitch, int processedPitch, int velocity) 
    : m_originalPitch(originalPitch), m_processedPitch(processedPitch), m_velocity(velocity) {
}

int ActiveNote::originalPitch() const {
    return m_originalPitch;
}

int ActiveNote::pitch() const {
    return m_processedPitch;
}

int ActiveNote::velocity() const {
    return m_velocity;
}

int Keyboard::getPitchClass(int pitch) const {
    return pitch % OCTAVE;
}

int Keyboard::clampPitchToRange(int pitch) {
    return std::max(rangeLow, std::min(pitch, rangeHigh));
}

int Keyboard::randomizeNote(int pitch) {
    if (pitch < 0 || pitch > 127) {
        return -1;
    }
    
    // No octave randomization - just return original pitch
    // Still apply range clamping for safety
    return clampPitchToRange(pitch);
}

Keyboard::Keyboard(int low, int high) : rangeLow(low), rangeHigh(high) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
    // Lock to full MIDI range 0-127
    rangeLow = 0;
    rangeHigh = 127;
}

int Keyboard::note(int pitch, int velocity) {
    std::cout << "Keyboard::note called - pitch: " << pitch << " velocity: " << velocity << std::endl;
    
    if (pitch < 0 || pitch > 127 || velocity < 0 || velocity > 127) {
        std::cout << "Invalid input - ignoring" << std::endl;
        return 0;
    }
    
    if (velocity > 0) {
        // Note ON - use original pitch (no octave changes)
        int processedPitch = randomizeNote(pitch);
        std::cout << "Original pitch: " << pitch << " -> Processed pitch: " << processedPitch << std::endl;
        
        if (processedPitch >= 0 && processedPitch <= 127) {
            activeNotes.push_back(std::make_unique<ActiveNote>(pitch, processedPitch, velocity));
            std::cout << "Note added to active notes. Total: " << activeNotes.size() << std::endl;
            return 1;
        }
    } else {
        // Note OFF
        std::cout << "Processing note off for pitch class: " << getPitchClass(pitch) << std::endl;
        return clearNotesByPitchClass(pitch);
    }
    
    return 0;
}

int Keyboard::clearNotesByPitchClass(int pitch) {
    int clearedCount = 0;
    int targetPitchClass = getPitchClass(pitch);
    
    for (auto it = activeNotes.begin(); it != activeNotes.end(); ) {
        if (getPitchClass((*it)->originalPitch()) == targetPitchClass) {
            it = activeNotes.erase(it);
            clearedCount++;
        } else {
            ++it;
        }
    }
    
    std::cout << "Cleared " << clearedCount << " notes with pitch class " << targetPitchClass << std::endl;
    return clearedCount;
}

int Keyboard::removeAll() {
    int count = activeNotes.size();
    activeNotes.clear();
    std::cout << "Removed all " << count << " notes" << std::endl;
    return count;
}

void Keyboard::updateRange(int low, int high) {
    rangeLow = low;
    rangeHigh = high;
    std::cout << "Range updated to: " << rangeLow << " - " << rangeHigh << std::endl;
}

void Keyboard::setRandomRange(int low, int high) {
    // This method is kept for compatibility but does nothing
    // since we're not doing octave randomization
    std::cout << "setRandomRange called but octave randomization is disabled" << std::endl;
}


const std::vector<std::unique_ptr<ActiveNote>>& Keyboard::getActiveNotes() const {
    return activeNotes;
}

void Keyboard::debugPrintActiveNotes() const {
    std::cout << "Active notes: " << activeNotes.size() << std::endl;
    for (const auto& note : activeNotes) {
        std::cout << "  Original: " << note->originalPitch() 
                  << " -> Processed: " << note->pitch() 
                  << " Velocity: " << note->velocity() << std::endl;
    }
}
