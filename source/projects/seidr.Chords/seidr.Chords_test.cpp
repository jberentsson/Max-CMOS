/// @file       seidr.Chords_test.cpp
///	@ingroup 	seidr
///	@copyright	Copyright 2025 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License
///             found in the License.md file.

#include "seidr.Chords.cpp" // NOLINT
#include "seidr.Chords.hpp"
#include <c74_min_unittest.h>
#include <cassert>

using namespace c74;
using namespace MIDI::Notes;

constexpr int NOTE_ON = 100;

SCENARIO("create an instance of ChordsMax") { // NOLINT
    ext_main(nullptr);

    GIVEN("An instance of our object") {
        min::test_wrapper<ChordsMax> an_instance;
        ChordsMax &myObject = an_instance;

        REQUIRE(true);
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
        myObject.anything({NoteC4, NOTE_ON});

        // A C Major chord.
        myObject.anything({NoteC4, NOTE_ON});
        myObject.anything({NoteE4, NOTE_ON});
        myObject.anything({NoteG4, NOTE_ON});

        REQUIRE(note_output.empty());
        REQUIRE(velocity_output.empty());

        // Release the chord.
        myObject.anything({NoteC4, 0});
        myObject.anything({NoteE4, 0});
        myObject.anything({NoteG4, 0});

        REQUIRE(note_output.empty());
        REQUIRE(velocity_output.empty());

        myObject.anything({NoteC4, NOTE_ON});

        REQUIRE(!note_output.empty());
        REQUIRE(!velocity_output.empty());
    }
}
