/// @file       seidr.NoteRandomOctave.hpp
///	@ingroup 	seidr
///	@copyright	Copyright 2025 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License
///             found in the License.md file.

#pragma once

#include "RandomOctave/RandomOctave.hpp"
#include <string>
#include <c74_min.h>

using namespace c74;

class RandomOctaveMax : public min::object<RandomOctaveMax> {
private:
    RandomOctave randomOctave_ = RandomOctave();

public:
    MIN_DESCRIPTION{"Randomize the octave of a MIDI note message."}; // NOLINT 
    MIN_TAGS{"seidr"};                                               // NOLINT 
    MIN_AUTHOR{"Jóhann Berentsson"};                                 // NOLINT 
    MIN_RELATED{"seidr.*"};                                          // NOLINT 

    explicit RandomOctaveMax(const min::atoms &args = {});

    auto processNoteMessage(int note, int velocity) -> void;
    auto clearAllNotesMessage() -> void;
    auto clearNoteMessage(int note) -> void;

    auto getActiveNotes() -> std::vector<std::shared_ptr<ActiveNote>> { return this->randomOctave_.getActiveNotes(); }
    auto getQueuedNotes() -> std::vector<std::shared_ptr<ActiveNote>> { return this->randomOctave_.getNoteQueue(); }
    
    bool isNoteNumber(const std::string& str, int& result) {
        try {
            size_t pos;
            result = std::stoi(str, &pos);
            return pos == str.length();
        } catch (...) {
            return false;
        }
    }

    // Inlets
    min::inlet<> input_note_velcoty {this, "(list) note, velocity"};

    // Outlets
    min::outlet<> output_note       {this, "(anything) pitch"};
    min::outlet<> output_velocity   {this, "(anything) velocity"};
    
    min::message<min::threadsafe::yes> anything {
        this, "anything", "Handle any input",
        MIN_FUNCTION {
            return {};
        }
    };
    
    min::message<min::threadsafe::yes> integerInput {
        this, "int", "Handle integer input",
        MIN_FUNCTION {
            return {};
        }
    };
    
    min::message<min::threadsafe::yes> floatInput {
        this, "float", "Handle integer input",
        MIN_FUNCTION {
            return {};
        }
    };
    
    min::message<min::threadsafe::yes> bangInput {
        this, "bang", "Handle bang input",
        MIN_FUNCTION {
            return {};
        }
    };

    min::message<min::threadsafe::yes> list {
        this, "list", "Process note messages",
        MIN_FUNCTION {
            if (args.size() >= 2) {
                int note = static_cast<int> (args[0]);
                int velocity = static_cast<int> (args[1]);
                this->processNoteMessage(note, velocity);
            }
            return {};
        }
    };

    min::message<min::threadsafe::yes> clear {
        this, "clear", "Clear specific note",
        MIN_FUNCTION {
            if (!args.empty()) {
                const std::string& arg = args[0];
                
                if (arg == "all") {
                    this->clearAllNotesMessage();
                } else {
                    int note;
                    if (this->isNoteNumber(arg, note)) {
                        this->clearNoteMessage(note);
                    } else {
                        std::cerr << "Error: Invalid argument. Use 'all' or a note number." << std::endl;
                    }
                }
            }
            return {};
        }
    };

    min::message<min::threadsafe::yes> range {
        this, "range", "Set range",
        MIN_FUNCTION {
            if (!args.empty() && args.size() >= 2) {
                int low = static_cast<int> (args[0]);
                int high = static_cast<int> (args[1]);
                this->randomOctave_.setRange(low, high);
            }
            return {};
        }
    };
};
