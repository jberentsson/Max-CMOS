/// @file       seidr.Quantizer.hpp
///	@ingroup 	seidr
///	@copyright	Copyright 2025 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License
///             found in the License.md file.

#pragma once

#include "Quantizer/Quantizer.hpp"
#include <c74_min.h>
#include <iostream>

using namespace c74::min;

class QuantizerMax : public object<QuantizerMax> {
public:
    MIN_DESCRIPTION{"Quantizer"};      // NOLINT 
    MIN_TAGS{"seidr"};                 // NOLINT 
    MIN_AUTHOR{"Jóhann Berentsson"};   // NOLINT 
    MIN_RELATED{"seidr.*"};            // NOLINT 

    explicit QuantizerMax(const atoms &args = {});

    auto processNote(int note, int velocity) -> void;
    //auto addNote(int noteValue) -> int { return this->quantizer.addNote(noteValue); }
    auto noteCount() -> int { return this->quantizer.noteCount(); }
    auto getRoundDirection() -> Quantizer::RoundDirection { return this->quantizer.getRoundDirection(); }
    auto setRoundDirection(Quantizer::RoundDirection direction) -> Quantizer::RoundDirection { return this->quantizer.setRoundDirection(direction); }
    auto setMode(Quantizer::QuantizeMode mode) -> Quantizer::QuantizeMode { return this->quantizer.setMode(mode); }

    inlet<> input_note {this, "(anything | list | reset) input note"};

    outlet<> output_note     {this, "(int) output note"};
    outlet<> output_velocity {this, "(int) output velocity"};

    message<threadsafe::yes> int_message { this, "int", "Post something to the Max console.",
        MIN_FUNCTION {
            return {};
        }
    };

    message<threadsafe::yes> anything {
        this, "anything", "Process note messages",
        MIN_FUNCTION {
            if (!args.empty()){
                cout << "ANYTHING: Arg.size(): " << args.size() << endl;
            }
            return {};
        }
    };
    
    message<threadsafe::yes> note_int {
        this, "int", "Process note messages",
        MIN_FUNCTION {
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
            if (!args.empty()) {
                for (const auto &arg : args) {
                    int note = static_cast<int>(arg);
                    if(note >= MIDI::RANGE_LOW && note <= MIDI::RANGE_HIGH ) {
                        this->quantizer.addNote(note);
                    }
                }                
            }
            return {};
        }
    };

    message<threadsafe::yes> quantizerThrough {
        this, "through", "Disable note through.",
        MIN_FUNCTION {
            if (!args.empty()) {
                switch(static_cast<int>(args[0])){
                    case 0:
                        this->quantizer.disableThrough();
                        break;
                    case 1:
                        this->quantizer.enableThrough();
                        break;
                }
            }
            return {};
        }
    };

    message<threadsafe::yes> quantizerClear {
        this, "clear", "Clear notes from the quantizer.",
        MIN_FUNCTION {
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
                    switch(static_cast<int>(arg)){
                        case 0:
                            this->quantizer.setMode(Quantizer::QuantizeMode::TWELVE_NOTES);
                            break; 
                        case 1:
                            this->quantizer.setMode(Quantizer::QuantizeMode::ALL_NOTES);
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
                int low = static_cast<int>(args[0]);
                int high = static_cast<int>(args[1]);
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
                    this->quantizer.deleteNote(static_cast<int>(arg));
                }
            }
            return {};
        }
    };

private:
    Quantizer quantizer;
};
