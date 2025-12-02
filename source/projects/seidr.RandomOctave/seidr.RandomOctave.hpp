/// @file       seidr.NoteRandomOctave.hpp
///	@ingroup 	seidr
///	@copyright	Copyright 2025 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License
///             found in the License.md file.

#pragma once

#include <c74_min.h>
#include "RandomOctave/RandomOctave.hpp"
#include <string>

using namespace c74::min;

class RandomOctaveMax : public object<RandomOctaveMax> {
private:
    RandomOctave randomOctave_;

public:
    MIN_DESCRIPTION{"Randomize the octave of a MIDI note message."}; // NOLINT 
    MIN_TAGS{"seidr"};                                               // NOLINT 
    MIN_AUTHOR{"Jóhann Berentsson"};                                 // NOLINT 
    MIN_RELATED{"seidr.*"};                                          // NOLINT 

    explicit RandomOctaveMax(const c74::min::atoms &args = {});

    auto processNoteMessage(int note, int velocity) -> void;
    auto clearAllNotesMessage() -> void;
    auto clearNoteMessage(int note) -> void;
    auto setRangeMessage(int low, int high) -> void;

    auto getActiveNotes() -> std::vector<std::shared_ptr<ActiveNote>> { return this->randomOctave_.getActiveNotes(); }
    auto getQueuedNotes() -> std::vector<std::shared_ptr<ActiveNote>> { return this->randomOctave_.getNoteQueue(); }

    // Inlets
    inlet<> input_note_velcoty {this, "(anything) note, (anything) velocity"};

    // Outlets
    outlet<> output_note       {this, "(anything) pitch"};
    outlet<> output_velocity   {this, "(anything) velocity"};

    message<> anything {
        this, "anything", "Process note messages",
        MIN_FUNCTION {
            c74::max::object_post((c74::max::t_object*)this, "anything");
            if (args.size() >= 2) {
                int note = args[0];
                int velocity = args[1];
                processNoteMessage(note, velocity);
                return {};
            }
            return {};
        }
    };
    
    message<> intInput {
        this, "int", "Process note messages",
        MIN_FUNCTION {
            c74::max::object_post((c74::max::t_object*)this, "int");
            if (args.size() >= 2) {
                int note = args[0];
                int velocity = args[1];
                processNoteMessage(note, velocity);
                return {};
            }
        }
    };
    
    message<> floatInput {
        this, "float", "Process note messages",
        MIN_FUNCTION {
            c74::max::object_post((c74::max::t_object*)this, "float");
            if (args.size() >= 2) {
                int note = args[0];
                int velocity = args[1];
                processNoteMessage(note, velocity);
                return {};
            }
            return {};
        }
    };

    c74::min::message<> clear{
        this, "clear", "Clear specific note",
        MIN_FUNCTION {
            if (!args.empty()) {
                if (static_cast<string>(args[0]) == "all") {
                    clearAllNotesMessage();
                } else {
                    int note = args[0];
                    clearNoteMessage(note);
                }
            }
            return {};
        }
    };

    c74::min::message<> range{
        this, "range", "Set range",
        MIN_FUNCTION {
            if (!args.empty() && args.size() >= 2) {
                int low = args[0];
                int high = args[1];
                this->randomOctave_.setRange(low, high);
            }
            return {};
        }
    };
};
