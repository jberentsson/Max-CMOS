/// @file       seidr.NoteRandomOctave.hpp
///	@ingroup 	seidr
///	@copyright	Copyright 2025 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License
///             found in the License.md file.

#pragma once

#include <c74_min.h>
#include "Keyboard/Keyboard.hpp"

// Forward declaration
class Keyboard;

class NoteRandomOctaveMax : public c74::min::object<NoteRandomOctaveMax> {
private:
    Keyboard keyboard_;

public:
    explicit NoteRandomOctaveMax(const c74::min::atoms &args = {});

    void processNoteMessage(int note, int velocity);
    void clearNoteMessage(int note);
    void clearAllNotesMessage();
    void setRangeMessage(int low, int high);

    // Inlets and outlets
    c74::min::inlet<> input0{this, "(int) note, (int) velocity"};
    c74::min::outlet<> output0{this, "(int) pitch"};
    c74::min::outlet<> output1{this, "(int) velocity"};

    // Messages with handlers defined inline
    c74::min::message<> anything{
        this, "anything", "Process note messages",
        MIN_FUNCTION {
            if (args.size() >= 2) {
                int note = args[0];
                int velocity = args[1];
                processNoteMessage(note, velocity);
            }
            return {};
        }
    };

    c74::min::message<> clear{
        this, "clear", "Clear specific note",
        MIN_FUNCTION {
            if (!args.empty()) {
                int note = args[0];
                clearNoteMessage(note);
            }
            return {};
        }
    };

    c74::min::message<> clearall{
        this, "clearall", "Clear all notes",
        MIN_FUNCTION {
            clearAllNotesMessage();
            return {};
        }
    };

    c74::min::message<> range{
        this, "range", "Set range",
        MIN_FUNCTION {
            if (!args.empty() && args.size() >= 2) {
                int low = args[0];
                int high = args[1];
                setRangeMessage(low, high);
            }
            return {};
        }
    };
};
