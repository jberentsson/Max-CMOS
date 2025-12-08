/// @file       seidr.Chords_test.cpp
///	@ingroup 	seidr
///	@copyright	Copyright 2025 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License
///             found in the License.md file.

#include "Utils/MIDI.hpp"
#include "seidr.Chords.cpp" // NOLINT
#include "seidr.Chords.hpp"
#include <c74_min_unittest.h>
#include <cassert>

using namespace c74;
using namespace MIDI::Notes;

constexpr int NOTE_ON = 100;
constexpr int NOTE_OFF = 0;

SCENARIO("create an instance of ChordsMax") { // NOLINT
    ext_main(nullptr);

    GIVEN("An instance of our object") {
        min::test_wrapper<ChordsMax> an_instance;
        ChordsMax &myObject = an_instance;

        REQUIRE(true);
    }

    GIVEN("many notes") {
        min::test_wrapper<ChordsMax> an_instance;
        ChordsMax &myObject = an_instance;

        auto &note_output = *max::object_getoutput(myObject, 0);
        auto &velocity_output = *max::object_getoutput(myObject, 1);

        for (int note = 0; note < MIDI::KEYBOARD_SIZE; note++) {
            myObject.list({ note, NOTE_ON });
            myObject.list({ note, NOTE_OFF });
        }

        REQUIRE(note_output.empty());
        REQUIRE(velocity_output.empty());
    }
}

SCENARIO("ChordsMax object produces correct output") { // NOLINT
    ext_main(nullptr);

    GIVEN("An instance of our object") {
        min::test_wrapper<ChordsMax> an_instance;
        ChordsMax &myObject = an_instance;

        auto &note_output = *max::object_getoutput(myObject, 0);
        auto &velocity_output = *max::object_getoutput(myObject, 1);

        myObject.recordNotes();

        REQUIRE(note_output.empty());
        REQUIRE(velocity_output.empty());

        // Pick a note.
        myObject.list({NoteC4, NOTE_ON});

        // A C Major chord.
        REQUIRE_NOTHROW(myObject.list({NoteC4, NOTE_ON}));
        REQUIRE_NOTHROW(myObject.list({NoteE4, NOTE_ON}));
        REQUIRE_NOTHROW(myObject.list({NoteG4, NOTE_ON}));

        REQUIRE(note_output.empty());
        REQUIRE(velocity_output.empty());

        // Release the chord.
        REQUIRE_NOTHROW(myObject.list({NoteC4, NOTE_OFF}));
        REQUIRE_NOTHROW(myObject.list({NoteE4, NOTE_OFF}));
        REQUIRE_NOTHROW(myObject.list({NoteG4, NOTE_OFF}));

        // Make sure there was no output.
        REQUIRE(note_output.empty());
        REQUIRE(velocity_output.empty());

        // The key we assigned the notes to.
        REQUIRE_NOTHROW(myObject.list({NoteC4, NOTE_ON}));

        // Check if the notes were outputed.
        REQUIRE(!note_output.empty());
        REQUIRE(!velocity_output.empty());

        // Nothing has been assigned to this key.
        REQUIRE_NOTHROW(myObject.list({NoteC5, NOTE_ON}));

        // Make sure the the empty key didn't trigger anything.
        REQUIRE(note_output.size() == 3);
        REQUIRE(velocity_output.size() == 3);

        // The key we assigned the notes to.
        REQUIRE_NOTHROW(myObject.list({NoteC4, NOTE_OFF}));

        // Make sure the notes are released.
        REQUIRE(note_output.size() == 6);
        REQUIRE(velocity_output.size() == 6);

    }
}
