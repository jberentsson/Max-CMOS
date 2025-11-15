#pragma once
#include "Keyboard/Keyboard.hpp"
#include <c74_min.h>

// Forward declaration
class Keyboard;

class NoteRandomOctave : public c74::min::object<NoteRandomOctave> {
    private:
        Keyboard keyboard = Keyboard();
        
    public:
        NoteRandomOctave(const c74::min::atoms& args = {});
        ~NoteRandomOctave();
        
        void processNoteMessage(int note, int velocity);
        void clearNoteMessage(int note);
        void clearAllNotesMessage();
        void setRangeMessage(int low, int high);
        void printActiveNotes();
        
        c74::min::inlet<>  input_0  { this, "(int) note, (int) velocity" };
        c74::min::outlet<> output_0 { this, "(int) pitch" };
        c74::min::outlet<> output_1 { this, "(int) velocity" };
        
        c74::min::message<> anything { this, "anything", "Handle any message",
            MIN_FUNCTION {
                if (args.size() >= 2) {
                    int note = args[0];
                    int velocity = args[1];
                    this->processNoteMessage(note, velocity);
                }
                return {};
            }
        };
        
        c74::min::message<> clear { this, "clear", "Clear specific note",
            MIN_FUNCTION {
                if (args.size() >= 1) {
                    int note = args[0];
                    this->clearNoteMessage(note);
                }
                return {};
            }
        };
        
        c74::min::message<> clearall { this, "clearall", "Clear all notes",
            MIN_FUNCTION {
                this->clearAllNotesMessage();
                return {};
            }
        };
        
        c74::min::message<> range { this, "range", "Set random range",
            MIN_FUNCTION {
                if (args.size() >= 2) {
                    int low = args[0];
                    int high = args[1];
                    this->setRangeMessage(low, high);
                }
                return {};
            }
        };
        
        c74::min::message<> debug { this, "debug", "Print active notes",
            MIN_FUNCTION {
                this->printActiveNotes();
                return {};
            }
        };
};