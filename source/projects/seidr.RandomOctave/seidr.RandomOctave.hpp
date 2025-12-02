/// @file       seidr.NoteRandomOctave.hpp
///	@ingroup 	seidr
///	@copyright	Copyright 2025 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License
///             found in the License.md file.

#pragma once

#include <c74_min.h>
#include "RandomOctave/RandomOctave.hpp"
#include <string>

using namespace c74;

class RandomOctaveMax : public min::object<RandomOctaveMax> {
private:
    RandomOctave randomOctave_;

public:
    MIN_DESCRIPTION{"Randomize the octave of a MIDI note message."}; // NOLINT 
    MIN_TAGS{"seidr"};                                               // NOLINT 
    MIN_AUTHOR{"Jóhann Berentsson"};                                 // NOLINT 
    MIN_RELATED{"seidr.*"};                                          // NOLINT 

    explicit RandomOctaveMax(const min::atoms &args = {});

    auto processNoteMessage(int note, int velocity) -> void;
    auto clearAllNotesMessage() -> void;
    auto clearNoteMessage(int note) -> void;
    auto setRangeMessage(int low, int high) -> void;

    auto getActiveNotes() -> std::vector<std::shared_ptr<ActiveNote>> { return this->randomOctave_.getActiveNotes(); }
    auto getQueuedNotes() -> std::vector<std::shared_ptr<ActiveNote>> { return this->randomOctave_.getNoteQueue(); }

    // Inlets
    min::inlet<> input_note_velcoty {this, "(list) note, velocity"};

    // Outlets
    min::outlet<> output_note       {this, "(anything) pitch"};
    min::outlet<> output_velocity   {this, "(anything) velocity"};
    
    min::message<> anything {
        this, "anything", "Handle any input",
        MIN_FUNCTION {
            max::object_post((max::t_object*) this, "anything");
            // Forward to list handler.
            //return list(args, inlet);
            return {};
        }
    };

    min::message<> list {
        this, "list", "Process note messages",
        MIN_FUNCTION {
            max::object_post((max::t_object*) this, "list");
            if (args.size() >= 2) {
                //int note = static_cast<int> (args[0]);
                //int velocity = static_cast<int> (args[1]);
                int note = int(args[0]);
                int velocity = int(args[1]);
                processNoteMessage(note, velocity);
            }
            return {};
        }
    };

    min::message<> clear{
        this, "clear", "Clear specific note",
        MIN_FUNCTION {
            if (!args.empty()) {
                if (args[0] == "all") {
                    clearAllNotesMessage();
                } else {
                    int note = static_cast<int> (args[0]);
                    clearNoteMessage(note);
                }
            }
            return {};
        }
    };

    min::message<> range{
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
