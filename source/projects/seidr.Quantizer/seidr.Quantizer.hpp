/// @file       seidr.Quantizer.hpp
///	@ingroup 	seidr
///	@copyright	Copyright 2025 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License
///             found in the License.md file.

#pragma once

#include "Quantizer/Quantizer.hpp"
#include <c74_min.h>

using namespace c74;

class QuantizerMax : public min::object<QuantizerMax> {
private:
    Quantizer quantizer;

public:
    MIN_DESCRIPTION{"Quantize a MIDI note message."}; // NOLINT 
    MIN_TAGS{"seidr"};                                // NOLINT 
    MIN_AUTHOR{"Jóhann Berentsson"};                  // NOLINT 
    MIN_RELATED{"seidr.*"};                           // NOLINT 

    explicit QuantizerMax(const min::atoms &args = {}) {
        if (!args.empty()) {
            // QuantizeMode
            if (!args.empty()) {
                this->quantizer.setMode(Quantizer::QuantizeMode(static_cast<int>(args[0])));
            }

            // RoundDirection
            if (args.size() >= 2) {
                this->quantizer.setRoundDirection(Quantizer::RoundDirection(static_cast<int>(args[1])));
            }

            // Range
            if (args.size() == 4) { 
                uint8_t rangeLow = static_cast<int>(args[2]);
                uint8_t rangeHigh = static_cast<int>(args[3]);
                this->quantizer.setRange(Quantizer::Note(rangeLow), Quantizer::Note(rangeHigh));
            }
        }
    }

    auto noteCount() -> int { return this->quantizer.noteCount(); }
    auto getRoundDirection() -> Quantizer::RoundDirection { return this->quantizer.getRoundDirection(); }
    
    // Process note implementation
    auto processNoteMessage(int notePitch, int velocity) -> void { // NOLINT
        // Validate input.
        if ((notePitch < MIDI::RANGE_LOW )|| (notePitch > MIDI::RANGE_HIGH)) {
            return;
        }
        
        // Quantize the note.
        int quantizedNote = quantizer.quantize(MIDI::Note(notePitch));
        
        // Send to outlets.
        output_note.send(quantizedNote);
        
        if (velocity <= MIDI::RANGE_HIGH) {
            output_velocity.send(velocity);
        }
    }

    // Inlets
    min::inlet<> input_note       {this, "(anything|add|delete|update|reset) input note"};

    // Outlets
    min::outlet<> output_note     {this, "(anything) output note"};
    min::outlet<> output_velocity {this, "(anything) output velocity"};
    min::outlet<> output_invalid  {this, "(bang) note was not playaed"};

    min::message<min::threadsafe::yes> int_message { this, "int", "Post something to the Max console.",
        MIN_FUNCTION {
            return {};
        }
    };

min::message<> anything {
    this, "anything", "Process note messages",
    MIN_FUNCTION {
        max::object_post((max::t_object*) this, "anything with %d args", args.size());
        
        // Check we have enough arguments
        if (args.size() >= 2) {
            int note = args[0];
            int velocity = args[1];
            processNoteMessage(note, velocity);
        } else {
            max::object_error((max::t_object*) this, "Need 1 or 2 arguments (note, [velocity])");
        }
        
        return {};
    }
};

    min::message<min::threadsafe::yes> quantizerAddNote {
        this, "add", "Add notes to quantizer",
        MIN_FUNCTION {
            max::object_post((max::t_object*)this, "add\n");
            if (!args.empty()) {
                for (const auto &arg : args) {
                    int note = static_cast<int> (arg);
                    if((note >= MIDI::RANGE_LOW) && (note <= MIDI::RANGE_HIGH) ) {
                        this->quantizer.addNote(MIDI::Note(note));
                    }
                }                
            }
            return {};
        }
    };

    min::message<min::threadsafe::yes> quantizerThrough {
        this, "through", "Disable note through.",
        MIN_FUNCTION {
            max::object_post((max::t_object*) this, "through\n");
            if (!args.empty()) {
                int quantizeFlag = static_cast<int> (args[0]);

                switch(quantizeFlag){
                    case 0:
                        this->quantizer.disableThrough();
                        break;
                    case 1:
                        this->quantizer.enableThrough();
                        break;
                    default:
                        break;
                }
            }
            return {};
        }
    };

    min::message<min::threadsafe::yes> updateNotes {
        this, "update", "Clears all notes",
        MIN_FUNCTION {
            max::object_post((max::t_object*) this, "update\n");
            if (!args.empty()) {
                this->quantizer.clear();
                
                for(const auto &argValue : args) {
                    int noteValue = (int)argValue;
                    this->quantizer.addNote(MIDI::Note(noteValue));  // <-- Or here?
                }
            }

            return {};
        }
    };

    min::message<min::threadsafe::yes> quantizerClear {
        this, "clear", "Clear notes from the quantizer.",
        MIN_FUNCTION {
            max::object_post((max::t_object*) this, "clear\n");
            if (!args.empty()) {
                    this->quantizer.clear();
            }
            return {};
        }
    };

    min::message<min::threadsafe::yes> quantizerMode {
        this, "mode", "Set quantizer mode.",
        MIN_FUNCTION {
            max::object_post((max::t_object*) this, "mode\n");
            if (!args.empty()) {
                for (const auto &arg : args) {
                    int modeFlag = static_cast<int> (arg);

                    switch(modeFlag){
                        case 0:
                            this->quantizer.setMode(Quantizer::QuantizeMode::TWELVE_NOTES);
                            break; 
                        case 1:
                            this->quantizer.setMode(Quantizer::QuantizeMode::ALL_NOTES);
                            break;
                        default:
                            break;
                    }
                }                
            }
            return {};
        }
    };

    min::message<min::threadsafe::yes> quantizerRound {
        this, "round", "Set quantizer mode.",
        MIN_FUNCTION {
            max::object_post((max::t_object*) this, "round\n");
            if (!args.empty()) {
                for (const auto &arg : args) {
                    int modeFlag = static_cast<int> (arg);
                    
                    // TODO: Can this be made in couple of lines?
                    switch(modeFlag){
                        case 0:
                            this->quantizer.setRoundDirection(Quantizer::RoundDirection::UP);
                            break; 
                        case 1:
                            this->quantizer.setRoundDirection(Quantizer::RoundDirection::DOWN);
                            break;
                        case 2:
                            this->quantizer.setRoundDirection(Quantizer::RoundDirection::UP_OVERFLOW);
                            break;
                        case 3:
                            this->quantizer.setRoundDirection(Quantizer::RoundDirection::DOWN_UNDERFLOW);
                            break;
                        case 4:
                            this->quantizer.setRoundDirection(Quantizer::RoundDirection::NEAREST);
                            break;
                        case 5: // NOLINT
                            this->quantizer.setRoundDirection(Quantizer::RoundDirection::FURTHEST);
                            break;
                        default:
                            break;
                    }
                }                
            }
            return {};
        }
    };

    min::message<min::threadsafe::yes> quantizerRange {
        this, "range", "Set quantizer range.",
        MIN_FUNCTION {
            if (!args.empty() && args.size() >= 2) {
                auto low = MIDI::Note(static_cast<int> (args[0]));
                auto high = MIDI::Note(static_cast<int> (args[1]));
                this->quantizer.setRange(low, high);
            }
            return {};
        }
    };

    min::message<min::threadsafe::yes> quantizerDeleteNote {
        this, "delete", "Delete notes from quantizer",
        MIN_FUNCTION {
            max::object_post((max::t_object*) this, "delete\n");
            if (!args.empty()){
                for(const auto &arg : args){
                    this->quantizer.deleteNote(MIDI::Note(static_cast<int> (arg)));
                }
            }
            return {};
        }
    };
};
