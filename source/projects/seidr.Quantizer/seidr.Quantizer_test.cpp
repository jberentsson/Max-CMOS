/// @file       seidr.Quantizer_test.cpp
///	@ingroup 	seidr
///	@copyright	Copyright 2025 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License
///             found in the License.md file.

#include <c74_min_unittest.h>
#include "seidr.Quantizer.hpp"
#include "seidr.Quantizer.cpp" // NOLINT

using namespace c74::min;
using namespace MIDI::Notes;

SCENARIO("object can be created") {
    ext_main(nullptr);
    GIVEN("A QuantizerMax instance") {
        test_wrapper<QuantizerMax> an_instance;
        QuantizerMax& myObject = an_instance;
        
        WHEN("the object is created") {
            THEN("it should be valid") {
                REQUIRE(true);
            }
        }
    }
}

SCENARIO("quantizer can add notes") {
    ext_main(nullptr);
    GIVEN("A QuantizerMax instance") {
        test_wrapper<QuantizerMax> an_instance;
        QuantizerMax& myObject = an_instance;
        myObject.quantizerMode(Quantizer::QuantizeMode::ALL_NOTES);
        
        WHEN("adding notes via addNote method") {
            // FIXED: Use the actual method names
            REQUIRE_NOTHROW(myObject.quantizerAddNote(NoteC4));
            REQUIRE(myObject.noteCount() == 1);
            
            REQUIRE_NOTHROW(myObject.quantizerAddNote(NoteC5));
            REQUIRE(myObject.noteCount() == 2);
        }
        
        WHEN("adding notes via add message") {
            atoms args = {NoteC4, NoteE4, NoteG4};
            // FIXED: Use the correct message handler name
            REQUIRE_NOTHROW(myObject.quantizerAddNote(args));
            REQUIRE(myObject.noteCount() == 3);
        }
    }
}

SCENARIO("quantizer processes notes correctly") {
    ext_main(nullptr);
    GIVEN("A QuantizerMax with C major scale") {
        test_wrapper<QuantizerMax> an_instance;
        QuantizerMax& myObject = an_instance;
        myObject.quantizerMode(Quantizer::QuantizeMode::ALL_NOTES);
        
        // Add C major notes
        atoms scale = {NoteC5, NoteD5, NoteE5, NoteF5, NoteG5, NoteA5, NoteB5};
        myObject.quantizerAddNote(scale);
        
        auto& out0 = *c74::max::object_getoutput(myObject, 0);
        auto& out1 = *c74::max::object_getoutput(myObject, 1);
        
        WHEN("processing a note with integer message") {
            atoms args = {NoteDS5}; // D#5 should quantize to E5
            REQUIRE_NOTHROW(myObject.note_int(args));
            
            THEN("output should be sent") {
                REQUIRE(!out0.empty());
            }
        }
        
        WHEN("processing a note with list message") {
            atoms args = {NoteDS5, 100}; // NOLINT
            REQUIRE_NOTHROW(myObject.list(args));
            
            THEN("both note and velocity outputs should be sent") {
                REQUIRE(!out0.empty());
                REQUIRE(!out1.empty());
            }
        }
    }
}

SCENARIO("quantizer handles note deletion") {
    ext_main(nullptr);
    GIVEN("A QuantizerMax with some notes") {
        test_wrapper<QuantizerMax> an_instance;
        QuantizerMax& myObject = an_instance;
        
        atoms notes = {NoteC5, NoteE5, NoteG5};
        
        myObject.quantizerMode(1);  // FIXED: Pass as atoms
        myObject.quantizerAddNote(notes);

        REQUIRE(myObject.noteCount() == 3);
        
        WHEN("deleting notes via del message") {
            atoms toDelete = {NoteE5};
            REQUIRE_NOTHROW(myObject.quantizerDeleteNote(toDelete));
            
            THEN("note count should decrease") {
                REQUIRE(myObject.noteCount() == 2);
            }
        }
    }
}

SCENARIO("quantizer handles round directions") {
    ext_main(nullptr);
    GIVEN("A QuantizerMax with specific notes") {
        test_wrapper<QuantizerMax> an_instance;
        QuantizerMax& myObject = an_instance;
        
        atoms notes = {NoteC5, NoteE5};
        myObject.quantizerAddNote(notes);
        
        WHEN("setting round direction") {
            REQUIRE_NOTHROW(myObject.quantizerRound(Quantizer::RoundDirection::UP));
            REQUIRE(myObject.getRoundDirection() == Quantizer::RoundDirection::UP);
            
            REQUIRE_NOTHROW(myObject.quantizerRound(Quantizer::RoundDirection::DOWN));
            REQUIRE(myObject.getRoundDirection() == Quantizer::RoundDirection::DOWN);
        }
    }
}

SCENARIO("quantizer handles range limits") {
    ext_main(nullptr);
    GIVEN("A QuantizerMax with range limits") {
        test_wrapper<QuantizerMax> an_instance;
        QuantizerMax& myObject = an_instance;
        
        myObject.quantizerMode(0);  // TWELVE_NOTES mode
        
        atoms notes = {NoteC5, NoteE5, NoteE6};
        myObject.quantizerAddNote(notes);

        int rangeLow = NoteC3;
        int rangeHigh = NoteC5;

        myObject.quantizerRange({rangeLow, rangeHigh});

        REQUIRE_NOTHROW(myObject.note_int(NoteB3));

        auto &note_output = *c74::max::object_getoutput(myObject, 0);
        auto &velocity_output = *c74::max::object_getoutput(myObject, 1);

        REQUIRE(!note_output.empty());
        REQUIRE(velocity_output.empty());
        REQUIRE(!note_output[0].empty());

        REQUIRE((int) note_output[0][1] >= NoteC3);
        REQUIRE((int) note_output[0][1] <= NoteC5);
    }
}

SCENARIO("adding notes to the quantizer") {
    ext_main(nullptr);

    test_wrapper<QuantizerMax> an_instance;
    QuantizerMax &myObject = an_instance;
    myObject.quantizerMode(Quantizer::QuantizeMode::ALL_NOTES);
    
    GIVEN("the notes"){
        auto &out = *c74::max::object_getoutput(myObject, 0);
        
        REQUIRE_NOTHROW(myObject.note_int(MIDI::Notes::NoteB3));
        REQUIRE(out[0][1] == MIDI::Notes::NoteB3);

        REQUIRE(!out.empty());
        REQUIRE_NOTHROW(myObject.quantizerAddNote(MIDI::Notes::NoteC4));
        
        REQUIRE_NOTHROW(myObject.note_int(MIDI::Notes::NoteC4));
        REQUIRE(!out.empty());
        REQUIRE(!out[0].empty());
        REQUIRE(out[1][1] == 48);

        WHEN("first note is detected") {
            REQUIRE(myObject.noteCount() == 1);
        }
        
        REQUIRE_NOTHROW(myObject.quantizerAddNote(MIDI::Notes::NoteC5));
        REQUIRE(myObject.noteCount() == 2);
    }
}

SCENARIO("adding notes to the quantizer with velocity") {
    ext_main(nullptr);

    test_wrapper<QuantizerMax> an_instance;
    QuantizerMax &myObject = an_instance;
    
    myObject.quantizerMode(Quantizer::QuantizeMode::ALL_NOTES);

    GIVEN("the notes") {
        REQUIRE(myObject.getRoundDirection() == Quantizer::RoundDirection::UP);

        auto &out0 = *c74::max::object_getoutput(myObject, 0);
        auto &out1 = *c74::max::object_getoutput(myObject, 1);

        c74::min::atoms args = {MIDI::Notes::NoteB3, 127}; // NOLINT
        REQUIRE_NOTHROW(myObject.list(args));

        REQUIRE(!out0.empty());
        REQUIRE(!out1.empty());
        REQUIRE(!out0[0].empty());
        REQUIRE(!out1[0].empty());
        REQUIRE(out0[0][1] == MIDI::Notes::NoteB3);
        REQUIRE(out1[0][1] == 127);
        REQUIRE_NOTHROW(myObject.quantizerAddNote({NoteB3, 69}));
        REQUIRE(myObject.noteCount() == 2);

        REQUIRE_NOTHROW(myObject.quantizerAddNote(NoteC5));
        REQUIRE(myObject.noteCount() == 3);

        c74::min::atoms args2 = {MIDI::Notes::NoteB4, 127}; // NOLINT
        REQUIRE_NOTHROW(myObject.list(args2));

        REQUIRE(!out0.empty());
        REQUIRE(!out1.empty());
        REQUIRE(!out0[1].empty());
        REQUIRE(!out1[1].empty());
        REQUIRE(out0[1][1] == MIDI::Notes::NoteC5);
        REQUIRE(out1[1][1] == 127);

        REQUIRE_NOTHROW(myObject.quantizerAddNote(NoteG4));
        REQUIRE(myObject.noteCount() == 4);
    
        REQUIRE_NOTHROW(myObject.quantizerRound(Quantizer::RoundDirection::DOWN));

        c74::min::atoms args3 = {MIDI::Notes::NoteB4, 127}; // NOLINT
        REQUIRE_NOTHROW(myObject.list(args3));

        REQUIRE(!out0.empty());
        REQUIRE(!out1.empty());
        REQUIRE(!out0[2].empty());
        REQUIRE(!out1[2].empty());
        REQUIRE(out0[2][1] == MIDI::Notes::NoteG4);
        REQUIRE(out1[2][1] == 127); // NOLINT
    }
}

SCENARIO("clear the notes") { // NOLINT
    ext_main(nullptr);
    
    GIVEN("OCTAVE MODE") {
        test_wrapper<QuantizerMax> an_instance;
        QuantizerMax &myObject = an_instance;
        
        myObject.quantizerMode(Quantizer::QuantizeMode::TWELVE_NOTES);

        WHEN("add and remove notes") {
            REQUIRE_NOTHROW(myObject.quantizerAddNote({0,1,2,3,4,5,6,7,8,9,10,11,12}));

            for (int i = 0; i <= 11; i++) { // NOLINT
                REQUIRE_NOTHROW(myObject.quantizerDeleteNote({i, -1}));
            }

            REQUIRE(myObject.noteCount() == 0);            
        }
    }
    
    GIVEN("ALL KEYS MODE") {
        test_wrapper<QuantizerMax> an_instance;
        QuantizerMax &myObject = an_instance;

        myObject.quantizerMode(Quantizer::QuantizeMode::ALL_NOTES);
        
        WHEN("add and remove notes") {
            REQUIRE(myObject.noteCount() == 0);

            for (int i = 0; i < MIDI::KEYBOARD_SIZE; i++) {
                REQUIRE(myObject.noteCount() == i);
                REQUIRE_NOTHROW(myObject.quantizerAddNote(i));
                REQUIRE(myObject.noteCount() == i + 1);
            }

            for (int i = 0; i < MIDI::KEYBOARD_SIZE; i++) {
                REQUIRE_NOTHROW(myObject.quantizerDeleteNote(i));
            }

            REQUIRE(myObject.noteCount() == 0);
        }
    }
}
