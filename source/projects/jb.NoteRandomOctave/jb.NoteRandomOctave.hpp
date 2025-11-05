/// @file       jb.NoteRandomOctave.hpp
///	@ingroup 	jb
///	@copyright	Copyright 2025 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License found in the License.md file.

#pragma once

#include "c74_min.h"
#include "Keyboard.hpp"

class Keyboard;
class ActiveNote;

using namespace c74::min;

class NoteRandomOctave : public object<NoteRandomOctave> {
    public:
        MIN_DESCRIPTION {"Randomize note octaves"};
        MIN_TAGS {"midi, notes, random"};
        MIN_AUTHOR {"Jóhann Berentsson"};
        MIN_RELATED {"jb.*"};

        // Inlets
        inlet<>  input  { this, "(list) note messages" };
        
        // Outlets  
        outlet<> output_0 { this, "(number) note" };
        outlet<> output_1 { this, "(number) velocity" };

        // Messages
        message<> NoteRandomOctave::note_list = { this, "list", "Process note messages.",
            MIN_FUNCTION {
                if (args.size() >= 1) {
                    int note = args[0];
                    int velocity = DEFAULT_VELOCITY;

                    if(args.size() == 2){
                        velocity = args[1];
                    }

                    output_0.send(note);
                    output_1.send(velocity);

                    std::vector<ActiveNote*> notes = keyboard->note(note, velocity);
                    
                    if (velocity == 0){
                        keyboard->clearNotes(note);
                    }
                }
                
                return {};
            }
        };

        // Range list
        message<> NoteRandomOctave::range_list = { this, "range", "Set the range of the notes.",
            MIN_FUNCTION {
                if (args.size() >= 2) {
                    keyboard->setRandomRange(args[0], args[1]);
                }
                return {};
            }
        };

        // Attributes
        attribute<symbol> output_type { 
            this, 
            "output_type", 
            "number",
            description {"Output type: note or velocity"}
        };

        NoteRandomOctave(const atoms& args = {});
        ~NoteRandomOctave();

    private:
        Keyboard* keyboard;
        static const int DEFAULT_VELOCITY = 127;
};
