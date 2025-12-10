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

        REQUIRE_NOTHROW(chordsTest.recordNotes("record", 1));

        REQUIRE(note_output.empty());
        REQUIRE(velocity_output.empty());

        // Pick a note.
        REQUIRE_NOTHROW(chordsTest.listInput({NoteC4, NOTE_ON}), 0);

        // A C Major chord.
        REQUIRE_NOTHROW(chordsTest.listInput({NoteC4, NOTE_ON}, 0));
        REQUIRE_NOTHROW(chordsTest.listInput({NoteE4, NOTE_ON}, 0));
        REQUIRE_NOTHROW(chordsTest.listInput({NoteG4, NOTE_ON}, 0));

        REQUIRE(note_output.empty());
        REQUIRE(velocity_output.empty());

        // Release the chord.
        REQUIRE_NOTHROW(chordsTest.listInput({NoteC4, NOTE_OFF}, 0));
        REQUIRE_NOTHROW(chordsTest.listInput({NoteE4, NOTE_OFF}, 0));
        REQUIRE_NOTHROW(chordsTest.listInput({NoteG4, NOTE_OFF}, 0));

        // Make sure there was no output.
        REQUIRE(note_output.empty());
        REQUIRE(velocity_output.empty());

        // The key we assigned the notes to.
        REQUIRE_NOTHROW(chordsTest.listInput({NoteC4, NOTE_ON}, 0));

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

        REQUIRE_NOTHROW(chordsTest.recordNotes("record", 1));

        REQUIRE_NOTHROW(chordsTest.listInput({NoteD4, NOTE_ON}));

        REQUIRE_NOTHROW(chordsTest.listInput({NoteC4, NOTE_ON}));
        REQUIRE_NOTHROW(chordsTest.listInput({NoteF4, NOTE_ON}));
        REQUIRE_NOTHROW(chordsTest.listInput({NoteA4, NOTE_ON}));

        REQUIRE_NOTHROW(chordsTest.listInput({NoteC4, NOTE_OFF}));
        REQUIRE_NOTHROW(chordsTest.listInput({NoteF4, NOTE_OFF}));
        REQUIRE_NOTHROW(chordsTest.listInput({NoteA4, NOTE_OFF}));

        REQUIRE_NOTHROW(chordsTest.listInput({NoteD4, NOTE_ON}));

        REQUIRE(note_output[6][1] == NoteC4);
        REQUIRE(note_output[7][1] == NoteF4);
        REQUIRE(note_output[8][1] == NoteA4);

        REQUIRE(velocity_output[6][1] == NOTE_ON);
        REQUIRE(velocity_output[7][1] == NOTE_ON);
        REQUIRE(velocity_output[8][1] == NOTE_ON);

        REQUIRE_NOTHROW(chordsTest.listInput({NoteC4, NOTE_ON}));

        REQUIRE(note_output[9][1] == NoteC4);
        REQUIRE(note_output[10][1] == NoteE4);
        REQUIRE(note_output[11][1] == NoteG4);

        REQUIRE(velocity_output[9][1] == NOTE_ON);
        REQUIRE(velocity_output[10][1] == NOTE_ON);
        REQUIRE(velocity_output[11][1] == NOTE_ON);

        REQUIRE_NOTHROW(chordsTest.listInput({NoteD4, NOTE_OFF}));

        REQUIRE(note_output[12][1] == NoteF4);
        REQUIRE(note_output[13][1] == NoteA4);

        REQUIRE(velocity_output[12][1] == NOTE_OFF);
        REQUIRE(velocity_output[13][1] == NOTE_OFF);
        
        REQUIRE_NOTHROW(chordsTest.listInput({NoteC4, NOTE_OFF}));

        REQUIRE(note_output[14][1] == NoteC4);
        REQUIRE(note_output[15][1] == NoteE4);
        REQUIRE(note_output[16][1] == NoteG4);

        REQUIRE(velocity_output[14][1] == NOTE_OFF);
        REQUIRE(velocity_output[15][1] == NOTE_OFF);
        REQUIRE(velocity_output[16][1] == NOTE_OFF);
    }
}

SCENARIO("ChordsMax object produces correct output second try") { // NOLINT
    ext_main(nullptr);

    GIVEN("An instance of our object") {
        min::test_wrapper<ChordsMax> an_instance;
        ChordsMax &chordsTest = an_instance;

        auto &note_output = *max::object_getoutput(chordsTest, 0);
        auto &velocity_output = *max::object_getoutput(chordsTest, 1);

        REQUIRE_NOTHROW(chordsTest.recordNotes("record", 1));

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

        REQUIRE_NOTHROW(chordsTest.recordNotes("record", 1));

        REQUIRE_NOTHROW(chordsTest.listInput({NoteD4, NOTE_ON}));

        REQUIRE_NOTHROW(chordsTest.listInput({NoteC4, NOTE_ON}));
        REQUIRE_NOTHROW(chordsTest.listInput({NoteF4, NOTE_ON}));
        REQUIRE_NOTHROW(chordsTest.listInput({NoteA4, NOTE_ON}));
        
        REQUIRE(note_output.empty());
        REQUIRE(velocity_output.empty());

        REQUIRE_NOTHROW(chordsTest.listInput({NoteC4, NOTE_OFF}));
        REQUIRE_NOTHROW(chordsTest.listInput({NoteF4, NOTE_OFF}));
        REQUIRE_NOTHROW(chordsTest.listInput({NoteA4, NOTE_OFF}));

        REQUIRE(note_output.empty());
        REQUIRE(velocity_output.empty());

        REQUIRE_NOTHROW(chordsTest.listInput({NoteD4, NOTE_ON}));

        REQUIRE(note_output.size() == 3);
        REQUIRE(velocity_output.size() == 3);

        REQUIRE(note_output[0][1] == NoteC4);
        REQUIRE(note_output[1][1] == NoteF4);
        REQUIRE(note_output[2][1] == NoteA4);

        REQUIRE(velocity_output[0][1] == NOTE_ON);
        REQUIRE(velocity_output[1][1] == NOTE_ON);
        REQUIRE(velocity_output[2][1] == NOTE_ON);

        REQUIRE_NOTHROW(chordsTest.listInput({NoteC4, NOTE_ON}));

        // We are retriggering even if note has already been sent.
        REQUIRE(note_output.size() == 6);
        REQUIRE(velocity_output.size() == 6);

        REQUIRE(note_output[4][1] == NoteE4);
        REQUIRE(note_output[5][1] == NoteG4);

        REQUIRE(velocity_output[4][1] == NOTE_ON);
        REQUIRE(velocity_output[5][1] == NOTE_ON);

        REQUIRE_NOTHROW(chordsTest.listInput({NoteD4, NOTE_OFF}));

        REQUIRE(note_output.size() == 8);
        REQUIRE(velocity_output.size() == 8);

        REQUIRE(note_output[6][1] == NoteF4);
        REQUIRE(note_output[7][1] == NoteA4);

        REQUIRE(velocity_output[6][1] == NOTE_OFF);
        REQUIRE(velocity_output[7][1] == NOTE_OFF);

        REQUIRE_NOTHROW(chordsTest.listInput({NoteD4, NOTE_ON}));

        REQUIRE(note_output.size() == 11);
        REQUIRE(velocity_output.size() == 11);

        REQUIRE(note_output[8][1] == NoteC4);
        REQUIRE(note_output[9][1] == NoteF4);
        REQUIRE(note_output[10][1] == NoteA4);

        REQUIRE(velocity_output[8][1] == NOTE_ON);
        REQUIRE(velocity_output[9][1] == NOTE_ON);
        REQUIRE(velocity_output[10][1] == NOTE_ON);

        REQUIRE_NOTHROW(chordsTest.listInput({NoteD4, NOTE_OFF}));

        REQUIRE(note_output.size() == 13);
        REQUIRE(velocity_output.size() == 13);

        REQUIRE(note_output[11][1] == NoteF4);
        REQUIRE(note_output[12][1] == NoteA4);

        REQUIRE(velocity_output[11][1] == NOTE_OFF);
        REQUIRE(velocity_output[12][1] == NOTE_OFF);

        REQUIRE_NOTHROW(chordsTest.recordNotes("record", 1));
        REQUIRE_NOTHROW(chordsTest.listInput({NoteE4, NOTE_ON}));  // C chord

        // Test this additional scenario:
        REQUIRE_NOTHROW(chordsTest.listInput({NoteC4, NOTE_ON}));  // C chord
        REQUIRE_NOTHROW(chordsTest.listInput({NoteE4, NOTE_ON}));
        REQUIRE_NOTHROW(chordsTest.listInput({NoteG4, NOTE_ON}));

        REQUIRE_NOTHROW(chordsTest.listInput({NoteC4, NOTE_OFF}));  // C chord
        REQUIRE_NOTHROW(chordsTest.listInput({NoteE4, NOTE_OFF}));
        REQUIRE_NOTHROW(chordsTest.listInput({NoteG4, NOTE_OFF}));

        REQUIRE_NOTHROW(chordsTest.listInput({NoteF4, NOTE_ON}));  // C chord

        REQUIRE_NOTHROW(chordsTest.listInput({NoteD4, NOTE_ON}));  // D chord (adds F, A)
        REQUIRE_NOTHROW(chordsTest.listInput({NoteF4, NOTE_ON}));
        REQUIRE_NOTHROW(chordsTest.listInput({NoteA4, NOTE_ON}));

        REQUIRE_NOTHROW(chordsTest.listInput({NoteD4, NOTE_OFF}));  // D chord (adds F, A)
        REQUIRE_NOTHROW(chordsTest.listInput({NoteF4, NOTE_OFF}));
        REQUIRE_NOTHROW(chordsTest.listInput({NoteA4, NOTE_OFF}));

        REQUIRE_NOTHROW(chordsTest.listInput({NoteE4, NOTE_ON}));
        REQUIRE_NOTHROW(chordsTest.listInput({NoteF4, NOTE_ON}));

        // Now release C4 - what happens to the shared F note?
        REQUIRE_NOTHROW(chordsTest.listInput({NoteC4, NOTE_OFF}));
        // Should F4 and A4 stay ON (from D chord)!
        // But your current implementation might turn them OFF
        REQUIRE(note_output[13][1] == NoteC4);
        REQUIRE(note_output[14][1] == NoteF4);
        REQUIRE(note_output[15][1] == NoteA4);

        REQUIRE(velocity_output[13][1] == NOTE_ON);
        REQUIRE(velocity_output[14][1] == NOTE_ON);
        REQUIRE(velocity_output[15][1] == NOTE_ON);

        REQUIRE(note_output[16][1] == NoteF4);
        REQUIRE(note_output[17][1] == NoteA4);

        REQUIRE(velocity_output[16][1] == NOTE_OFF);
        REQUIRE(velocity_output[17][1] == NOTE_OFF);

        REQUIRE(note_output[18][1] == NoteC4);
        REQUIRE(note_output[19][1] == NoteE4);
        REQUIRE(note_output[20][1] == NoteG4);

        REQUIRE(velocity_output[18][1] == NOTE_ON);
        REQUIRE(velocity_output[19][1] == NOTE_ON);
        REQUIRE(velocity_output[20][1] == NOTE_ON);

    }
}

SCENARIO("Shared notes in different order") {
    ext_main(nullptr);

    GIVEN("An instance of our object") {
        min::test_wrapper<ChordsMax> an_instance;
        ChordsMax &chordsTest = an_instance;

        auto &note_output = *max::object_getoutput(chordsTest, 0);
        auto &velocity_output = *max::object_getoutput(chordsTest, 1);

        // Test the reverse order

        // C Major
        chordsTest.recordNotes("record", 1);
        chordsTest.listInput({NoteC4, NOTE_ON});  // C chord first
        chordsTest.listInput({NoteC4, NOTE_OFF});

        chordsTest.listInput({NoteC4, NOTE_ON});  // Record C chord
        chordsTest.listInput({NoteE4, NOTE_ON});
        chordsTest.listInput({NoteG4, NOTE_ON});

        chordsTest.listInput({NoteC4, NOTE_OFF});  // Record C chord
        chordsTest.listInput({NoteE4, NOTE_OFF});
        chordsTest.listInput({NoteG4, NOTE_OFF});
        
        // F Major
        chordsTest.recordNotes("record", 1);
        chordsTest.listInput({NoteD4, NOTE_ON});  // F chord second
        chordsTest.listInput({NoteD4, NOTE_OFF});

        chordsTest.listInput({NoteC4, NOTE_ON});  // Record F chord
        chordsTest.listInput({NoteF4, NOTE_ON});
        chordsTest.listInput({NoteA4, NOTE_ON});

        chordsTest.listInput({NoteC4, NOTE_OFF});  // Record F chord
        chordsTest.listInput({NoteF4, NOTE_OFF});
        chordsTest.listInput({NoteA4, NOTE_OFF});
        
        // Play in opposite order
        chordsTest.listInput({NoteC4, NOTE_ON});  // C chord first
        
        REQUIRE(!note_output.empty());

        REQUIRE(note_output[0][1] == NoteC4);
        REQUIRE(note_output[1][1] == NoteE4);
        REQUIRE(note_output[2][1] == NoteG4);

        chordsTest.listInput({NoteD4, NOTE_ON});  // F chord second

        REQUIRE(note_output[3][1] == NoteC4);
        REQUIRE(note_output[4][1] == NoteF4);
        REQUIRE(note_output[5][1] == NoteA4);
        
        chordsTest.listInput({NoteC4, NOTE_OFF});  // Release C chord
        
        REQUIRE(note_output[6][1] == NoteE4);
        REQUIRE(note_output[7][1] == NoteG4);

        chordsTest.listInput({NoteD4, NOTE_OFF});  // Release F chord

        REQUIRE(note_output[8][1] == NoteC4);
        REQUIRE(note_output[9][1] == NoteF4);
        REQUIRE(note_output[10][1] == NoteA4);
    }
}

SCENARIO("enable/disable") { // NOLINT
    ext_main(nullptr);

    min::test_wrapper<ChordsMax> an_instance;
    ChordsMax &chordsTest = an_instance;

    auto &note_output = *max::object_getoutput(chordsTest, 0);
    auto &velocity_output = *max::object_getoutput(chordsTest, 1);

    GIVEN("sending a note in when enabled") {
        REQUIRE_NOTHROW(chordsTest.enable("enable", 1));

        REQUIRE_NOTHROW(chordsTest.listInput({NoteB5, NOTE_ON}, 0));
        REQUIRE_NOTHROW(chordsTest.listInput({NoteB5, NOTE_OFF}, 0));

        REQUIRE(note_output.empty());
        REQUIRE(velocity_output.empty());
    }

    GIVEN("sending a note in when disabled") {
        REQUIRE_NOTHROW(chordsTest.disable("disable", 1));

        REQUIRE_NOTHROW(chordsTest.listInput({NoteB5, NOTE_ON}, 0));

        REQUIRE(velocity_output.size() == 1);
        REQUIRE(note_output.size() == 1);

        REQUIRE_NOTHROW(chordsTest.listInput({NoteB5, NOTE_OFF}, 0));

        REQUIRE(velocity_output.size() == 2);
        REQUIRE(note_output.size() == 2);
    }
}
