/// @file       seidr.Chords.hpp
///	@ingroup 	seidr
///	@copyright	Copyright 2025 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License found in the License.md file.

#pragma once

#include <iostream>
#include "Chords/Chords.hpp"
#include <c74_min.h>

using namespace c74;

class ChordsMax : public min::object<ChordsMax> {
public:
    MIN_DESCRIPTION{"Chords"};       // NOLINT 
    MIN_TAGS{"seidr"};               // NOLINT 
    MIN_AUTHOR{"Jóhann Berentsson"}; // NOLINT 
    MIN_RELATED{"seidr.*"};          // NOLINT 

    enum Inlets : int { // NOLINT
        NOTES = 0,
        ARGS = 1
    };

    min::inlet<> input_notes{this, "(list) note velocity"};
    min::inlet<> input_args{this, "(record) arguments"};
    
    min::outlet<> output_note          {this, "(anything) output note"};
    min::outlet<> output_velocity      {this, "(anything) output velocity"};
    min::outlet<> output_note_velocity {this, "(anything) output note and velocity"};
    
    ChordsMax(const min::atoms &args = {}) {};

    min::message<min::threadsafe::yes> floatInput {this, "float", "Recive note input.",
        MIN_FUNCTION{
            std::cout << "inlet " << inlet << "\n";
            return this->listInput(args);
        }
    };

    min::message<min::threadsafe::yes> intInput {this, "int", "Recive note input.",
        MIN_FUNCTION{
            std::cout << "inlet " << inlet << "\n";
            return this->listInput(args);
        }
    };

    min::message<min::threadsafe::yes> listInput {this, "list", "Recive note input.",
        MIN_FUNCTION{
            std::cout << "inlet " << inlet << "\n";

            if(!args.empty() && (args.size() == 2) && (inlet == 0)) {
                int pitchValue = static_cast<int>(args[0]);
                int velocityValue = static_cast<int>(args[1]);

                if(this->enabled_) {
                    // If the chord memory is enabled.
                    this->chords_.note(pitchValue, velocityValue);

                    // Send out the notes on the note queue.
                    for(const auto &currentNote : this->chords_.noteQueue()) {
                        output_velocity.send(static_cast<int>(currentNote->velocity()));
                        output_note.send(static_cast<int>(currentNote->pitch()));
                        output_note_velocity.send({
                            static_cast<int>(currentNote->pitch()),
                            static_cast<int>(currentNote->velocity())
                        });
                    }

                    this->chords_.noteQueue().clear();
                } else {
                    // Send the notes through if the chord memory has been disabled.
                    output_velocity.send(velocityValue);
                    output_note.send(pitchValue);
                    output_note_velocity.send({
                        pitchValue,
                        velocityValue
                    });
                }
            }

            return {};
        }
    };

    min::message<min::threadsafe::yes> recordNotes {this, "record", "record",
        MIN_FUNCTION{
            std::cout << "inlet " << inlet << "\n";
            if (inlet == 1) {
                this->chords_.reciveNotes();
            }

            return {};
        }
    };

    min::message<min::threadsafe::yes> enable {this, "enable", "enable the object",
        MIN_FUNCTION{
            std::cout << "inlet " << inlet << "\n";
            if (inlet == 1) {
                this->enabled_ = true;
            }

            return {};
        }
    };

    min::message<min::threadsafe::yes> disable {this, "disable", "disable the object",
        MIN_FUNCTION{
            std::cout << "inlet " << inlet << "\n";
            if (inlet == 1) {
                this->enabled_ = false;;
            }

            return {};
        }
    };

private:
    Chords chords_;
    bool enabled_ = true;
};
