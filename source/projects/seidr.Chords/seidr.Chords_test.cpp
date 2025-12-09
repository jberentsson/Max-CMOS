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
        ChordsMax &chordsTest = an_instance;

        REQUIRE(true);
    }

    GIVEN("many notes") {
        min::test_wrapper<ChordsMax> an_instance;
        ChordsMax &chordsTest = an_instance;

        auto &note_output = *max::object_getoutput(chordsTest, 0);
        auto &velocity_output = *max::object_getoutput(chordsTest, 1);

        for (int note = 0; note < MIDI::KEYBOARD_SIZE; note++) {
            chordsTest.listInput({ note, NOTE_ON });
            chordsTest.listInput({ note, NOTE_OFF });
        }

        REQUIRE(note_output.empty());
        REQUIRE(velocity_output.empty());
    }
}

SCENARIO("ChordsMax object produces correct output") { // NOLINT
    ext_main(nullptr);

    GIVEN("An instance of our object") {
        min::test_wrapper<ChordsMax> an_instance;
        ChordsMax &chordsTest = an_instance;

        auto &note_output = *max::object_getoutput(chordsTest, 0);
        auto &velocity_output = *max::object_getoutput(chordsTest, 1);

        REQUIRE_NOTHROW(chordsTest.recordNotes("record"));

        REQUIRE(note_output.empty());
        REQUIRE(velocity_output.empty());

        // Pick a note.
        REQUIRE_NOTHROW(chordsTest.listInput({NoteC4, NOTE_ON}));

        // A C Major chord.
        REQUIRE_NOTHROW(chordsTest.listInput({NoteC4, NOTE_ON}));
        REQUIRE_NOTHROW(chordsTest.listInput({NoteE4, NOTE_ON}));
        REQUIRE_NOTHROW(chordsTest.listInput({NoteG4, NOTE_ON}));

        REQUIRE(note_output.empty());
        REQUIRE(velocity_output.empty());

        // Release the chord.
        REQUIRE_NOTHROW(chordsTest.listInput({NoteC4, NOTE_OFF}));
        REQUIRE_NOTHROW(chordsTest.listInput({NoteE4, NOTE_OFF}));
        REQUIRE_NOTHROW(chordsTest.listInput({NoteG4, NOTE_OFF}));

        // Make sure there was no output.
        REQUIRE(note_output.empty());
        REQUIRE(velocity_output.empty());

        // The key we assigned the notes to.
        REQUIRE_NOTHROW(chordsTest.listInput({NoteC4, NOTE_ON}));

        // Check if the notes were outputed.
        REQUIRE(!note_output.empty());
        REQUIRE(!velocity_output.empty());

        // Nothing has been assigned to this key.
        REQUIRE_NOTHROW(chordsTest.listInput({NoteC5, NOTE_ON}));

        // Make sure the the empty key didn't trigger anything.
        REQUIRE(note_output.size() == 3);
        REQUIRE(velocity_output.size() == 3);

        // The key we assigned the notes to.
        REQUIRE_NOTHROW(chordsTest.listInput({NoteC4, NOTE_OFF}));

        // Make sure the notes are released.
        REQUIRE(note_output.size() == 6);
        REQUIRE(velocity_output.size() == 6);

        // Make sure the output is correct

        // NOTE ON
        REQUIRE(note_output[0][1] == NoteC4);
        REQUIRE(note_output[1][1] == NoteE4);
        REQUIRE(note_output[2][1] == NoteG4);

        REQUIRE(velocity_output[0][1] == NOTE_ON);
        REQUIRE(velocity_output[1][1] == NOTE_ON);
        REQUIRE(velocity_output[2][1] == NOTE_ON);
        
        // NOTE OFF
        REQUIRE(note_output[3][1] == NoteC4);
        REQUIRE(note_output[4][1] == NoteE4);
        REQUIRE(note_output[5][1] == NoteG4);

        REQUIRE(velocity_output[3][1] == NOTE_OFF);
        REQUIRE(velocity_output[4][1] == NOTE_OFF);
        REQUIRE(velocity_output[5][1] == NOTE_OFF);
    }
}

SCENARIO("enable/disable") { // NOLINT
    ext_main(nullptr);

    min::test_wrapper<ChordsMax> an_instance;
    ChordsMax &chordsTest = an_instance;

    auto &note_output = *max::object_getoutput(chordsTest, 0);
    auto &velocity_output = *max::object_getoutput(chordsTest, 1);

    GIVEN("sending a note in when enabled") {
        REQUIRE_NOTHROW(chordsTest.enable());

        REQUIRE_NOTHROW(chordsTest.listInput({NoteB5, NOTE_ON}));
        REQUIRE_NOTHROW(chordsTest.listInput({NoteB5, NOTE_OFF}));

        REQUIRE(note_output.empty());
        REQUIRE(velocity_output.empty());
    }

    GIVEN("sending a note in when disabled") {
        REQUIRE_NOTHROW(chordsTest.disable());

        REQUIRE_NOTHROW(chordsTest.listInput({NoteB5, NOTE_ON}));

        REQUIRE(velocity_output.size() == 1);
        REQUIRE(note_output.size() == 1);

        REQUIRE_NOTHROW(chordsTest.listInput({NoteB5, NOTE_OFF}));

        REQUIRE(velocity_output.size() == 2);
        REQUIRE(note_output.size() == 2);
    }
}
