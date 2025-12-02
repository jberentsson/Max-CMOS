/// @file       seidr.Quantizer.hpp
///	@ingroup 	seidr
///	@copyright	Copyright 2025 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License
///             found in the License.md file.

#pragma once

#include "Quantizer/Quantizer.hpp"
#include <c74_min.h>

using namespace c74::min;

class QuantizerMax : public object<QuantizerMax> {
private:
    Quantizer quantizer;

public:
    MIN_DESCRIPTION{"Quantize a MIDI note message."}; // NOLINT 
    MIN_TAGS{"seidr"};                                // NOLINT 
    MIN_AUTHOR{"Jóhann Berentsson"};                  // NOLINT 
    MIN_RELATED{"seidr.*"};                           // NOLINT 

    explicit QuantizerMax(const atoms &args = {}) {
        if (!args.empty()) {
            // QuantizeMode
            if (!args.empty()) {
                this->quantizer.setMode(Quantizer::QuantizeMode(args[0]));
            }

            // RoundDirection
            if (args.size() >= 2) {
                this->quantizer.setRoundDirection(Quantizer::RoundDirection(args[1]));
            }

            // Range
            if (args.size() == 4) { 
                uint8_t rangeLow = int(args[2]);
                uint8_t rangeHigh = int(args[3]);
                this->quantizer.setRange(Quantizer::Note(rangeLow), Quantizer::Note(rangeHigh));
            }
        }
    }

    auto noteCount() -> int { return this->quantizer.noteCount(); }
    auto getRoundDirection() -> Quantizer::RoundDirection { return this->quantizer.getRoundDirection(); }
    
    // Process note implementation
    auto processNote(int notePitch, int velocity) -> void { // NOLINT
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

    inlet<> input_note {this, "(anything | list | reset) input note"};

    outlet<> output_note     {this, "(int) output note"};
    outlet<> output_velocity {this, "(int) output velocity"};
    outlet<> output_invalid  {this, "(bang) note was not playaed"};

    message<threadsafe::yes> int_message { this, "int", "Post something to the Max console.",
        MIN_FUNCTION {
            return {};
        }
    };

    message<threadsafe::yes> anything {
        this, "anything", "Process note messages",
        MIN_FUNCTION {
            cout << "anything\n";
            if (!args.empty()){
                cout << "ANYTHING: Arg.size(): " << args.size() << endl;
            }
            return {};
        }
    };
    
    message<threadsafe::yes> noteInput {
        this, "int", "Process note messages",
        MIN_FUNCTION {
            cout << "int\n";
            if (!args.empty()) {
                int note = static_cast<int>(args[0]);
                this->processNote(note, MIDI::RANGE_HIGH + 1);
            }
            return {};
        }
    };
    
    message<threadsafe::yes> noteInputFloat {
        this, "float", "Process note messages",
        MIN_FUNCTION {
            cout << "float\n";
            if (!args.empty()) {
                int note = static_cast<int>(args[0]);
                this->processNote(note, MIDI::RANGE_HIGH + 1);
            }
            return {};
        }
    };

    message<threadsafe::yes> list {
        this, "list", "Process note messages",
        MIN_FUNCTION {
            cout << "list\n";
            if (!args.empty() && args.size() == 2){
                int note = static_cast<int>(args[0]);
                int velocity = static_cast<int>(args[1]);
                this->processNote(note, velocity);
            }
            return {};
        }
    };

    message<threadsafe::yes> quantizerAddNote {
        this, "add", "Add notes to quantizer",
        MIN_FUNCTION {
            cout << "add\n";
            if (!args.empty()) {
                for (const auto &arg : args) {
                    int note = static_cast<int>(arg);
                    if(note >= MIDI::RANGE_LOW && note <= MIDI::RANGE_HIGH ) {
                        this->quantizer.addNote(MIDI::Note(note));
                    }
                }                
            }
            return {};
        }
    };

    message<threadsafe::yes> quantizerThrough {
        this, "through", "Disable note through.",
        MIN_FUNCTION {
            cout << "through\n";
            if (!args.empty()) {
                int quantizeFlag = static_cast<int>(args[0]);

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

    message<threadsafe::yes> updateNotes {
        this, "update", "Clears all of the notes currently set and adds the new ones.",
        MIN_FUNCTION {
            cout << "update\n";
            if (!args.empty()) {
                this->quantizer.clear();
                
                for(const auto &argValue : args) {
                    this->quantizer.addNote(MIDI::Note(static_cast<int> (argValue)));
                }
            }
            return {};
        }
    };

    message<threadsafe::yes> quantizerClear {
        this, "clear", "Clear notes from the quantizer.",
        MIN_FUNCTION {
            cout << "clear\n";
            if (!args.empty()) {
                    this->quantizer.clear();
            }
            return {};
        }
    };

    message<threadsafe::yes> quantizerMode {
        this, "mode", "Set quantizer mode.",
        MIN_FUNCTION {
            if (!args.empty()) {
                for (const auto &arg : args) {
                    int modeFlag = static_cast<int>(arg);

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

    message<threadsafe::yes> quantizerRound {
        this, "round", "Set quantizer mode.",
        MIN_FUNCTION {
            if (!args.empty()) {
                for (const auto &arg : args) {
                    int modeFlag = static_cast<int>(arg);

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

    message<threadsafe::yes> quantizerRange {
        this, "range", "Set quantizer range.",
        MIN_FUNCTION {
            if (!args.empty() && args.size() >= 2) {
                //int low = static_cast<int>(args[0]);
                //int high = static_cast<int>(args[1]);
                auto low = MIDI::Note(static_cast<int> (args[0]));
                auto high = MIDI::Note(static_cast<int> (args[1]));
                this->quantizer.setRange(low, high);
            }
            return {};
        }
    };

    message<threadsafe::yes> quantizerDeleteNote {
        this, "delete", "Delete notes from quantizer",
        MIN_FUNCTION {
            if (!args.empty()){
                for(const auto &arg: args){
                    this->quantizer.deleteNote(MIDI::Note(static_cast<int> (arg)));
                }
            }
            return {};
        }
    };
};
