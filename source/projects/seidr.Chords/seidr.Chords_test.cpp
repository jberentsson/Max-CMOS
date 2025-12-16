/// @file       seidr.Chords_test.cpp
///	@ingroup 	seidr
///	@copyright	Copyright 2025 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License
///             found in the License.md file.

#include <cassert>
#include <c74_min_unittest.h>
#include "Utils/MIDI.hpp"
#include "seidr.Chords.cpp" // NOLINT
#include "seidr.Chords.hpp"
#include "Utils/TestHelpers.hpp"

using namespace c74;
using namespace MIDI::Notes;

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

        REQUIRE(note_output.empty());
        REQUIRE(velocity_output.empty());

        // Pick a note.
        REQUIRE_NOTHROW(chordsTest.setKey({ NoteC4, NoteC4, NoteE4, NoteG4 }, 1)); // C Major chord.
        REQUIRE_NOTHROW(chordsTest.setKey({ NoteD4, NoteD4, NoteF4, NoteA4 }, 1)); // D Major chord.

        // Press some keys.
        REQUIRE_NOTHROW(chordsTest.listInput({ NoteC4, NOTE_ON  }, 0)); // C Major ON
        REQUIRE_NOTHROW(chordsTest.listInput({ NoteC5, NOTE_ON  }, 0)); // No Chord Assigned To Key
        REQUIRE_NOTHROW(chordsTest.listInput({ NoteC4, NOTE_OFF }, 0)); // C Major OFF
        REQUIRE_NOTHROW(chordsTest.listInput({ NoteD4, NOTE_ON  }, 0)); // D Major ON
        REQUIRE_NOTHROW(chordsTest.listInput({ NoteC4, NOTE_ON  }, 0)); // C Major ON
        REQUIRE_NOTHROW(chordsTest.listInput({ NoteD4, NOTE_OFF }, 0)); // D Major OFF
        REQUIRE_NOTHROW(chordsTest.listInput({ NoteC4, NOTE_OFF }, 0)); // C Major OFF

        int expectedOutput[][2] = {
            { NoteC4, NOTE_ON  }, // C Major ON
            { NoteE4, NOTE_ON  },
            { NoteG4, NOTE_ON  },
            { NoteC4, NOTE_OFF }, // C Major OFF
            { NoteE4, NOTE_OFF },
            { NoteG4, NOTE_OFF },
            { NoteD4, NOTE_ON  }, // D Major ON
            { NoteF4, NOTE_ON  },
            { NoteA4, NOTE_ON  },
            { NoteC4, NOTE_ON  }, // C Major ON
            { NoteE4, NOTE_ON  },
            { NoteG4, NOTE_ON  },
            { NoteD4, NOTE_OFF }, // D Major OFF
            { NoteF4, NOTE_OFF },
            { NoteA4, NOTE_OFF },
            { NoteC4, NOTE_OFF }, // C Major OFF
            { NoteE4, NOTE_OFF },
            { NoteG4, NOTE_OFF }
        };

        for (int i = 0; i < note_output.size(); i++) {
            REQUIRE(note_output[i][1] == expectedOutput[i][0]);
            REQUIRE(velocity_output[i][1] == expectedOutput[i][1]);
        }

        //REQUIRE(velocity_output.size() == 18);
        //REQUIRE(note_output.size() == 18);
    }
}

SCENARIO("ChordsMax object produces correct output second try") { // NOLINT
    ext_main(nullptr);

    min::test_wrapper<ChordsMax> an_instance;
    ChordsMax &chordsTest = an_instance;

    REQUIRE_NOTHROW(chordsTest.noteMode(ChordsMax::NoteMode::RETRIGGER, 1));
    REQUIRE(chordsTest.getNoteMode() == ChordsMax::NoteMode::RETRIGGER);

    auto &note_output = *max::object_getoutput(chordsTest, 0);
    auto &velocity_output = *max::object_getoutput(chordsTest, 1);

    GIVEN("An instance of our object") {
        REQUIRE(note_output.empty());
        REQUIRE(velocity_output.empty());

        REQUIRE_NOTHROW(chordsTest.setKey({ NoteC4, NoteC4, NoteE4, NoteG4 }, 1));  // C chord
        REQUIRE_NOTHROW(chordsTest.setKey({ NoteD4, NoteC4, NoteF4, NoteA4 }, 1));  // F chord
        REQUIRE_NOTHROW(chordsTest.setKey({ NoteE4, NoteC4, NoteE4, NoteA4 }, 1));  // A chord
        REQUIRE_NOTHROW(chordsTest.setKey({ NoteF4, NoteD4, NoteF4, NoteA4 }, 1));  // D chord

        // Make sure there was no output.
        REQUIRE(note_output.empty());
        REQUIRE(velocity_output.empty());

        WHEN("xx"){
            REQUIRE_NOTHROW(chordsTest.listInput({ NoteD4, NOTE_ON  }, 0)); // F Major ON
            REQUIRE_NOTHROW(chordsTest.listInput({ NoteC4, NOTE_ON  }, 0)); // C Major ON
            REQUIRE_NOTHROW(chordsTest.listInput({ NoteD4, NOTE_OFF }, 0)); // F Major OFF
            REQUIRE_NOTHROW(chordsTest.listInput({ NoteD4, NOTE_ON  }, 0)); // F Major ON
            REQUIRE_NOTHROW(chordsTest.listInput({ NoteD4, NOTE_OFF }, 0)); // F Major OFF
            REQUIRE_NOTHROW(chordsTest.listInput({ NoteE4, NOTE_ON  }, 0)); // A Major ON
            REQUIRE_NOTHROW(chordsTest.listInput({ NoteF4, NOTE_ON  }, 0)); // D Major OFF
            REQUIRE_NOTHROW(chordsTest.listInput({ NoteC4, NOTE_OFF }, 0));  // C Major OFF

            REQUIRE(note_output.size() == 20);
            REQUIRE(velocity_output.size() == 20);

            THEN("axax"){
                REQUIRE(note_output[0][1] == NoteC4); // F Major ON
                REQUIRE(note_output[1][1] == NoteF4);
                REQUIRE(note_output[2][1] == NoteA4);
                REQUIRE(note_output[3][1] == NoteC4); // C Major ON
                REQUIRE(note_output[4][1] == NoteE4);
                REQUIRE(note_output[5][1] == NoteG4);
                REQUIRE(note_output[6][1] == NoteF4); // F Major OFF
                REQUIRE(note_output[7][1] == NoteA4);
                REQUIRE(note_output[8][1] == NoteC4); // F Major ON
                REQUIRE(note_output[9][1] == NoteF4);
                REQUIRE(note_output[10][1] == NoteA4);
                REQUIRE(note_output[11][1] == NoteF4); // F Major OFF
                REQUIRE(note_output[12][1] == NoteA4);
                REQUIRE(note_output[13][1] == NoteC4); // A Major ON
                REQUIRE(note_output[14][1] == NoteE4);
                REQUIRE(note_output[15][1] == NoteA4);
                REQUIRE(note_output[16][1] == NoteD4); // D Minor OFF
                REQUIRE(note_output[17][1] == NoteF4);
                REQUIRE(note_output[18][1] == NoteA4);
                REQUIRE(note_output[19][1] == NoteG4); // C Major OFF
                //REQUIRE(note_output[20][1] == NoteE4);
                //REQUIRE(note_output[21][1] == NoteG4);
                //REQUIRE(note_output[22][1] == NoteE4);
                //REQUIRE(note_output[23][1] == NoteG4);

                REQUIRE(velocity_output[0][1] == NOTE_ON); // F Major ON
                REQUIRE(velocity_output[1][1] == NOTE_ON);
                REQUIRE(velocity_output[2][1] == NOTE_ON);
                REQUIRE(velocity_output[3][1] == NOTE_ON); // C Major ON
                REQUIRE(velocity_output[4][1] == NOTE_ON);
                REQUIRE(velocity_output[5][1] == NOTE_ON);
                REQUIRE(velocity_output[6][1] == NOTE_OFF); // F Major OFF
                REQUIRE(velocity_output[7][1] == NOTE_OFF);
                REQUIRE(velocity_output[8][1] == NOTE_ON); // F Major ON
                REQUIRE(velocity_output[9][1] == NOTE_ON);
                REQUIRE(velocity_output[10][1] == NOTE_ON);
                REQUIRE(velocity_output[11][1] == NOTE_OFF); // F Major OFF
                REQUIRE(velocity_output[12][1] == NOTE_OFF);
                REQUIRE(velocity_output[13][1] == NOTE_ON); // A Major ON
                REQUIRE(velocity_output[14][1] == NOTE_ON);
                REQUIRE(velocity_output[15][1] == NOTE_ON);
                REQUIRE(velocity_output[16][1] == NOTE_ON); // D Minor OFF
                REQUIRE(velocity_output[17][1] == NOTE_ON);
                REQUIRE(velocity_output[18][1] == NOTE_ON);
                REQUIRE(velocity_output[19][1] == NOTE_OFF); // C Major OFF
                //REQUIRE(velocity_output[20][1] == NOTE_ON);
                //REQUIRE(velocity_output[21][1] == NOTE_OFF);
                //REQUIRE(velocity_output[22][1] == NOTE_OFF);
                //REQUIRE(velocity_output[23][1] == NOTE_OFF);
            }
        }
    }
}

SCENARIO("Shared notes in different order NoteMode::RETRIGGER") {
    ext_main(nullptr);

    min::test_wrapper<ChordsMax> an_instance;
    ChordsMax &chordsTest = an_instance;

    auto &note_output = *max::object_getoutput(chordsTest, 0);
    auto &velocity_output = *max::object_getoutput(chordsTest, 1);

    REQUIRE_NOTHROW(chordsTest.noteMode(ChordsMax::NoteMode::RETRIGGER, 1));
    REQUIRE(chordsTest.getNoteMode() == ChordsMax::NoteMode::RETRIGGER);

    GIVEN("An instance of our object") {
        REQUIRE_NOTHROW(chordsTest.setKey({ NoteC4, NoteC4, NoteE4, NoteG4 }, 1)); // C Major
        REQUIRE_NOTHROW(chordsTest.setKey({ NoteD4, NoteC4, NoteF4, NoteA4 }, 1)); // F Major
        REQUIRE_NOTHROW(chordsTest.setKey({ NoteE4, NoteC4, NoteE4, NoteA4 }, 1)); // A Major

        WHEN("press couple of keys") {    
            // Play in opposite order
            REQUIRE_NOTHROW(chordsTest.listInput({ NoteC4, NOTE_ON }, 0));  // C chord first
            REQUIRE_NOTHROW(chordsTest.listInput({ NoteD4, NOTE_ON }, 0));  // F chord second
            REQUIRE_NOTHROW(chordsTest.listInput({ NoteE4, NOTE_ON }, 0));  // A chord
            REQUIRE_NOTHROW(chordsTest.listInput({ NoteE4, NOTE_OFF }, 0)); // Release A chord
            
            REQUIRE(!note_output.empty());

            THEN("make sure the output is correct") {
                REQUIRE(note_output.size() == 9);
                REQUIRE(velocity_output.size() == 9);

                REQUIRE( note_output[0][1] == NoteC4 ); // C Major ON
                REQUIRE( note_output[1][1] == NoteE4 ); 
                REQUIRE( note_output[2][1] == NoteG4 );
                REQUIRE( note_output[3][1] == NoteC4 ); // F Major ON
                REQUIRE( note_output[4][1] == NoteF4 );
                REQUIRE( note_output[5][1] == NoteA4 );
                REQUIRE( note_output[6][1] == NoteC4 ); // A Major ON
                REQUIRE( note_output[7][1] == NoteE4 );
                REQUIRE( note_output[8][1] == NoteA4 );
                //{ NoteC4, NOTE_OFF}, // A Major OFF
                //{ NoteE4, NOTE_OFF},
                //{ NoteA4, NOTE_OFF}

                REQUIRE( velocity_output[0][1] == NOTE_ON );
                REQUIRE( velocity_output[1][1] == NOTE_ON );
                REQUIRE( velocity_output[2][1] == NOTE_ON );
                REQUIRE( velocity_output[3][1] == NOTE_ON );
                REQUIRE( velocity_output[4][1] == NOTE_ON );
                REQUIRE( velocity_output[5][1] == NOTE_ON );
                REQUIRE( velocity_output[6][1] == NOTE_ON );
                REQUIRE( velocity_output[7][1] == NOTE_ON );
                REQUIRE( velocity_output[8][1] == NOTE_ON );
                //{ NoteC4, NOTE_OFF}, // A Major OFF
                //{ NoteE4, NOTE_OFF},
                //{ NoteA4, NOTE_OFF}
            }
        }
    }
}

SCENARIO("Shared notes in different order NoteMode::LEGATO") {
    ext_main(nullptr);
    
    min::test_wrapper<ChordsMax> an_instance;
    ChordsMax &chordsTest = an_instance;

    auto &note_output = *max::object_getoutput(chordsTest, 0);
    auto &velocity_output = *max::object_getoutput(chordsTest, 1);

    REQUIRE_NOTHROW(chordsTest.noteMode(ChordsMax::NoteMode::LEGATO, 1));
    REQUIRE(chordsTest.getNoteMode() == ChordsMax::NoteMode::LEGATO);

    GIVEN("An instance of our object") {
        REQUIRE_NOTHROW(chordsTest.setKey({ NoteC4, NoteC4, NoteE4, NoteG4 }, 1)); // C Major
        REQUIRE_NOTHROW(chordsTest.setKey({ NoteD4, NoteD4, NoteF4, NoteA4 }, 1)); // D Minor
        REQUIRE_NOTHROW(chordsTest.setKey({ NoteE4, NoteC4, NoteE4, NoteA4 }, 1)); // A Major

        WHEN("ad") {
            // Play in opposite order.
            REQUIRE_NOTHROW(chordsTest.listInput({ NoteC4, NOTE_ON  })); // C Major ON
            REQUIRE_NOTHROW(chordsTest.listInput({ NoteD4, NOTE_ON  })); // D Minor ON
            REQUIRE_NOTHROW(chordsTest.listInput({ NoteE4, NOTE_ON  })); // A Major ON
            REQUIRE_NOTHROW(chordsTest.listInput({ NoteE4, NOTE_OFF })); // A Major OFF
            REQUIRE_NOTHROW(chordsTest.listInput({ NoteD4, NOTE_OFF })); // D Minor OFF
            
            REQUIRE(!note_output.empty());
            
            REQUIRE(note_output[0][1] == NoteC4); // C Major ON
            REQUIRE(note_output[1][1] == NoteE4);
            REQUIRE(note_output[2][1] == NoteG4);
            REQUIRE(note_output[3][1] == NoteD4); // D Minor ON
            REQUIRE(note_output[4][1] == NoteF4);
            REQUIRE(note_output[5][1] == NoteA4);

            REQUIRE(velocity_output[0][1] == NOTE_ON); // C Major ON
            REQUIRE(velocity_output[1][1] == NOTE_ON);
            REQUIRE(velocity_output[2][1] == NOTE_ON);
            REQUIRE(velocity_output[3][1] == NOTE_ON); // D Minor ON
            REQUIRE(velocity_output[4][1] == NOTE_ON);
            REQUIRE(velocity_output[5][1] == NOTE_ON);

            //REQUIRE(note_output.size() == 6);
            //REQUIRE(velocity_output.size() == 6);
        }
    }
};

SCENARIO("enable/disable") { // NOLINT
    ext_main(nullptr);

    min::test_wrapper<ChordsMax> an_instance;
    ChordsMax &chordsTest = an_instance;

    auto &note_output = *max::object_getoutput(chordsTest, 0);
    auto &velocity_output = *max::object_getoutput(chordsTest, 1);

    GIVEN("sending a note in when enabled") {
        REQUIRE_NOTHROW(chordsTest.enableChords("enable", 1));

        REQUIRE_NOTHROW(chordsTest.listInput({NoteB5, NOTE_ON}, 0));
        REQUIRE_NOTHROW(chordsTest.listInput({NoteB5, NOTE_OFF}, 0));

        REQUIRE(note_output.empty());
        REQUIRE(velocity_output.empty());
    }

    GIVEN("sending a note in when disabled") {
        REQUIRE_NOTHROW(chordsTest.disableChords("disable", 1));

        REQUIRE_NOTHROW(chordsTest.listInput({NoteB5, NOTE_ON}, 0));

        REQUIRE(velocity_output.size() == 1);
        REQUIRE(note_output.size() == 1);

        REQUIRE_NOTHROW(chordsTest.listInput({NoteB5, NOTE_OFF}, 0));

        REQUIRE(velocity_output.size() == 2);
        REQUIRE(note_output.size() == 2);
    }
};

SCENARIO("set key notes") { // NOLINT
    ext_main(nullptr);

    min::test_wrapper<ChordsMax> an_instance;
    ChordsMax &chordsTest = an_instance;

    auto &note_output = *max::object_getoutput(chordsTest, 0);
    auto &velocity_output = *max::object_getoutput(chordsTest, 1);

    GIVEN("sending a note in when enabled") {
        REQUIRE_NOTHROW(chordsTest.setKey({NoteC4, NoteF4, NoteA4, NoteD5, NoteE5}, 1));
        
        REQUIRE(note_output.empty());
        REQUIRE(velocity_output.empty());
        
        REQUIRE_NOTHROW(chordsTest.listInput({NoteC4, NOTE_ON}, 0));  // F chord second
        
        REQUIRE(!note_output.empty());
        REQUIRE(!velocity_output.empty());

        REQUIRE(note_output.size() == 4);
        REQUIRE(velocity_output.size() == 4);
        
        REQUIRE_NOTHROW(chordsTest.listInput({NoteC4, NOTE_OFF}, 0));  // F chord second
        
        REQUIRE(note_output.size() == 8);
        REQUIRE(velocity_output.size() == 8);
    }
};
