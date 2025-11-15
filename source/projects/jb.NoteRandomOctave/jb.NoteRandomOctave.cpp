#include "jb.NoteRandomOctave.hpp"
#include <iostream>

using namespace c74::min;

void NoteRandomOctave::clearNoteMessage(int note) {
    if (keyboard) {
        std::cout << "Clearing note: " << note << std::endl;
        int clearedCount = keyboard->clearNotesByPitchClass(note);
        std::cout << "Cleared " << clearedCount << " notes" << std::endl;
        
        if (clearedCount > 0) {
            output_0.send(note);
            output_1.send(0);
            std::cout << "Sent note off: " << note << std::endl;
        }
    }
}

void NoteRandomOctave::clearAllNotesMessage() {
    std::cout << "Clearing ALL notes" << std::endl;
    
    if (keyboard) {
        int clearedCount = keyboard->removeAll();
        std::cout << "Cleared all " << clearedCount << " notes" << std::endl;
        
        if (clearedCount > 0) {
            // Send all notes off as fallback
            for (int note = 0; note < 128; note++) {
                output_0.send(note);
                output_1.send(0);
            }
            std::cout << "Sent all notes off" << std::endl;
        }
    } else {
        // Send all notes off as fallback
        for (int note = 0; note < 128; note++) {
            output_0.send(note);
            output_1.send(0);
        }
        std::cout << "Keyboard null - sent all notes off" << std::endl;
    }
}

void NoteRandomOctave::setRangeMessage(int low, int high) {
    std::cout << "Setting range: " << low << " to " << high << std::endl;
    if (keyboard) {
        keyboard->setRandomRange(low, high);
    }
}

void NoteRandomOctave::printActiveNotes() {
    if (keyboard) {
        keyboard->debugPrintActiveNotes();
    }
}

NoteRandomOctave::NoteRandomOctave(const atoms& args) {
    std::cout << "=== NoteRandomOctave constructor called ===" << std::endl;
    keyboard = new Keyboard();
    std::cout << "Keyboard created at address: " << keyboard << std::endl;
    
    // Test the outputs immediately
    std::cout << "Testing outputs with note 60..." << std::endl;
    output_0.send(60);
    output_1.send(100);
    std::cout << "Test note sent - check if you see output in Max" << std::endl;
}

NoteRandomOctave::~NoteRandomOctave() {
    std::cout << "NoteRandomOctave destructor called" << std::endl;
    clearAllNotesMessage();
    
    if (keyboard) {
        delete keyboard;
        keyboard = nullptr;
    }
}

void NoteRandomOctave::processNoteMessage(int note, int velocity) {
    std::cout << "=== NoteRandomOctave::processNoteMessage ===" << std::endl;
    std::cout << "INPUT: note=" << note << ", velocity=" << velocity << std::endl;
    
    if (!keyboard) {
        std::cout << "CRITICAL ERROR: keyboard is NULL!" << std::endl;
        return;
    }
    
    std::cout << "Keyboard pointer: " << keyboard << " (should not be zero)" << std::endl;
    
    // Process the note
    int result = keyboard->note(note, velocity);
    std::cout << "Keyboard::note returned: " << result << std::endl;
    
    if (velocity > 0) {
        // Note ON
        std::cout << "Processing NOTE ON..." << std::endl;
        const auto& activeNotes = keyboard->getActiveNotes();
        std::cout << "Active notes count: " << activeNotes.size() << std::endl;
        
        if (activeNotes.empty()) {
            std::cout << "WARNING: No active notes after processing!" << std::endl;
            return;
        }
        
        // Get the most recently added note
        const auto& lastNote = activeNotes.back();
        std::cout << "Last note - Original: " << lastNote->originalPitch() 
                  << " -> Processed: " << lastNote->pitch() 
                  << " Velocity: " << lastNote->velocity() << std::endl;
        
        // Send to outputs
        std::cout << "SENDING TO OUTPUTS - pitch: " << lastNote->pitch() 
                  << " velocity: " << lastNote->velocity() << std::endl;
        
        output_0.send(lastNote->pitch());
        output_1.send(lastNote->velocity());
        
        std::cout << "Outputs sent successfully!" << std::endl;
        
    } else {
        // Note OFF  
        std::cout << "Processing NOTE OFF..." << std::endl;
        std::cout << "SENDING NOTE OFF - pitch: " << note << " velocity: 0" << std::endl;
        
        output_0.send(note);
        output_1.send(0);
        
        std::cout << "Note off sent successfully!" << std::endl;
    }
    
    std::cout << "=== processNoteMessage complete ===" << std::endl;
}

MIN_EXTERNAL(NoteRandomOctave);