/// @file       seidr.Chords.hpp
///	@ingroup 	seidr
///	@copyright	Copyright 2025 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License found in the License.md file.

#pragma once

#include "Chords/Chords.hpp"
#include <c74_min.h>

using namespace c74;

class ChordsMax : public min::object<ChordsMax> {
public:
    MIN_DESCRIPTION{"Chords"};       // NOLINT 
    MIN_TAGS{"seidr"};               // NOLINT 
    MIN_AUTHOR{"Jóhann Berentsson"}; // NOLINT 
    MIN_RELATED{"seidr.*"};          // NOLINT 

    min::inlet<> input_notes{this, "(list) note velocity"};
    min::inlet<> input_args{this, "(record) arguments"};
    
    min::outlet<> output_note     {this, "(anything) output note"};
    min::outlet<> output_velocity {this, "(anything) output velocity"};
    
    ChordsMax(const min::atoms &args = {}) {};

    min::message<min::threadsafe::yes> floatInput {this, "float", "Recive note input.",
        MIN_FUNCTION{
            return this->listInput(args);
        }
    };

    min::message<min::threadsafe::yes> intInput {this, "int", "Recive note input.",
        MIN_FUNCTION{
            return this->listInput(args);
        }
    };

    min::message<min::threadsafe::yes> listInput {this, "list", "Recive note input.",
        MIN_FUNCTION{
            int pitchValue = static_cast<int>(args[0]);
            int velocityValue = static_cast<int>(args[1]);

            this->chords_.note(pitchValue, velocityValue);

            // TODO: Make it handle legato playing.

            // Send out the notes on the note queue.
            for(const auto &currentNote : this->chords_.noteQueue()) {
                output_velocity.send(velocityValue);

                // TODO: Move this into  the library.
                output_note.send(static_cast<int>(currentNote->pitch()));
            }

            this->chords_.noteQueue().clear();

            return {};
        }
    };

    min::message<min::threadsafe::yes> recordNotes {this, "record", "record",
        MIN_FUNCTION{
            this->chords_.reciveNotes();
            return {};
        }
    };

private:
    Chords chords_;
};
