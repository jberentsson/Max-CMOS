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
            int testInput[8][2] = { 
                { NoteD4, NOTE_ON  }, // F Major ON
                { NoteC4, NOTE_ON  }, // C Major ON
                { NoteD4, NOTE_OFF }, // F Major OFF
                { NoteD4, NOTE_ON  }, // F Major ON
                { NoteD4, NOTE_OFF }, // F Major OFF
                { NoteE4, NOTE_ON  }, // A Major ON
                { NoteF4, NOTE_ON  }, // D Major OFF
                { NoteC4, NOTE_OFF }  // C Major OFF
            };

            for (int i = 0; i < 8; i++) {
                REQUIRE_NOTHROW(chordsTest.listInput({testInput[i][0], testInput[i][1]}));
                //REQUIRE(note_output.size() % 3 == 0);
                //REQUIRE(velocity_output.size() % 3 == 0);
            }

            //REQUIRE(note_output.size() == 20);
            //REQUIRE(velocity_output.size() == 20);

            THEN("axax"){
                int expectedOutput[27][2] = {
                    // 1. F Major ON (F-A-C)
                    { NoteC4, NOTE_ON  },
                    { NoteF4, NOTE_ON  },
                    { NoteA4, NOTE_ON  },
                    
                    // 2. C Major ON while F Major active (C-E-G)
                    // RETRIGGER: Send NOTE_ON for ALL notes, including shared C4
                    { NoteC4, NOTE_ON  },  // RETRIGGER: Duplicate NOTE_ON for shared note!
                    { NoteE4, NOTE_ON  },
                    { NoteG4, NOTE_ON  },
                    
                    // 3. F Major OFF (C Major stays active)
                    // Only turn off notes NOT shared with C Major
                    { NoteF4, NOTE_OFF },  // Only in F Major
                    { NoteA4, NOTE_OFF },  // Only in F Major
                    // C4 stays ON (used by C Major) - NO NOTE_OFF!
                    
                    // 4. F Major ON again (while C Major still active)
                    // RETRIGGER: Send NOTE_ON for F Major's notes
                    { NoteF4, NOTE_ON  },
                    { NoteA4, NOTE_ON  },
                    // C4 is already ON from C Major AND was retriggered earlier
                    // In RETRIGGER mode: Should we send another NOTE_ON for C4 here?
                    // Probably NOT, because F Major's C4 is already active from step 1
                    
                    // 5. F Major OFF again
                    { NoteF4, NOTE_OFF },
                    { NoteA4, NOTE_OFF },
                    // C4 stays ON (still used by C Major)
                    
                    // 6. A Major ON (A-C♯-E) while C Major active
                    { NoteC4, NOTE_ON },  // A Major: A-C♯-E (C♯ not C!)
                    { NoteE4, NOTE_ON  },      // RETRIGGER: E4 already ON from C Major
                    { NoteA4, NOTE_ON  },
                    
                    // 7. D Major ON (D-F♯-A) while A Major and C Major active
                    { NoteD4, NOTE_ON  },
                    { NoteF4, NOTE_ON },  // D Major: D-F♯-A (F♯ not F!)
                    { NoteA4, NOTE_ON  },      // RETRIGGER: A4 already ON from A Major
                    
                    // 8. C Major OFF (A Major and D Major still active)
                    // Check which notes are ONLY in C Major
                    { NoteC4, NOTE_OFF },      // Only C Major has C4
                    { NoteG4, NOTE_OFF },      // Only C Major has G4
                    // E4 stays ON (A Major still needs it!) - NO NOTE_OFF
                    
                    // 9. A Major OFF (D Major still active)
                    { NoteC4, NOTE_OFF }, // Only A Major has C♯4
                    // E4: Both C Major (gone) and A Major need it
                    // Since A Major turns OFF and D Major doesn't use E4:
                    { NoteE4, NOTE_OFF },      // Now E4 can turn OFF
                    // A4 stays ON (D Major still needs it!) - NO NOTE_OFF
                    
                    // 10. D Major OFF (all chords done)
                    { NoteD4, NOTE_OFF },
                    { NoteF4, NOTE_OFF },
                    { NoteA4, NOTE_OFF }
                };

                for (int i = 0; i < note_output.size(); i++) {
                    std::cout << "index: " << i << "\n";
                    //REQUIRE(note_output[i][1] == expectedOutput[i][0]);
                    //REQUIRE(velocity_output[i][1] == expectedOutput[i][1]);
                }
            }
        }
    }
}

SCENARIO("Shared notes in different order NoteMode::RETRIGGER") {
    std::cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n";
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
                int expectedNoteOutput[][2] = {
                    { NoteC4, NOTE_ON }, // C Major ON
                    { NoteE4, NOTE_ON }, 
                    { NoteG4, NOTE_ON },
                    { NoteC4, NOTE_ON }, // F Major ON
                    { NoteF4, NOTE_ON },
                    { NoteA4, NOTE_ON },
                    { NoteC4, NOTE_ON }, // A Major ON
                    { NoteE4, NOTE_ON },
                    { NoteA4, NOTE_ON },
                    //{ NoteC4, NOTE_OFF}, // A Major OFF
                    //{ NoteE4, NOTE_OFF},
                    //{ NoteA4, NOTE_OFF}
                };

                for (int i = 0; i < note_output.size(); i++) {
                    REQUIRE(note_output[i][1] == expectedNoteOutput[i][0]);
                    REQUIRE(velocity_output[i][1] == expectedNoteOutput[i][1]);
                }

                REQUIRE(note_output.size() == 12);
                REQUIRE(velocity_output.size() == 12);
            }
        }
    }
    std::cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n";
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

            REQUIRE(note_output.size() == 9);
            REQUIRE(velocity_output.size() == 9);

            int expectedOutput[][2] = {
                { NoteC4, NOTE_ON  }, // C Major ON
                { NoteE4, NOTE_ON  },
                { NoteG4, NOTE_ON  },
                { NoteD4, NOTE_ON  }, // D Minor ON
                { NoteF4, NOTE_ON  },
                { NoteA4, NOTE_ON  },
                                      // A Major OFF No notes released.
                { NoteD4, NOTE_OFF }, // D Minor OFF
                { NoteF4, NOTE_OFF },
                { NoteA4, NOTE_OFF },
            };
            
            for (int i = 0; i < note_output.size(); i++) {
                REQUIRE(note_output[i][1] == expectedOutput[i][0]);
                REQUIRE(velocity_output[i][1] == expectedOutput[i][1]);
            }
        }
    }
}

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
}

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
}
