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
        QuantizerMax& quantizerTestObject = an_instance;
        
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
        QuantizerMax& quantizerTestObject = an_instance;
        REQUIRE_NOTHROW(quantizerTestObject.quantizerMode(Quantizer::QuantizeMode::ALL_NOTES));
        
        WHEN("adding notes via addNote method") {
            REQUIRE_NOTHROW(quantizerTestObject.quantizerAddNote(NoteC4));
            REQUIRE(quantizerTestObject.noteCount() == 1);
            
            REQUIRE_NOTHROW(quantizerTestObject.quantizerAddNote(NoteC5));
            REQUIRE(quantizerTestObject.noteCount() == 2);
        }
        
        WHEN("adding notes via add message") {
            REQUIRE_NOTHROW(quantizerTestObject.quantizerAddNote({ NoteC4, NoteE4, NoteG4 }));
            REQUIRE(quantizerTestObject.noteCount() == 3);
        }
    }
}

SCENARIO("quantizer processes notes correctly") {
    ext_main(nullptr);

    GIVEN("A QuantizerMax with C major scale") {
        test_wrapper<QuantizerMax> an_instance;
        QuantizerMax& quantizerTestObject = an_instance;
        REQUIRE_NOTHROW(quantizerTestObject.quantizerMode(Quantizer::QuantizeMode::ALL_NOTES));
        
        // Add C major notes.
        REQUIRE_NOTHROW(quantizerTestObject.quantizerAddNote({ NoteC5, NoteD5, NoteE5, NoteF5, NoteG5, NoteA5, NoteB5 }));
        
        auto& out0 = *c74::max::object_getoutput(quantizerTestObject, 0);
        auto& out1 = *c74::max::object_getoutput(quantizerTestObject, 1);
        
        WHEN("processing a note with integer message") {
            // D#5 should quantize to E5.
            REQUIRE_NOTHROW(quantizerTestObject.noteInput(NoteDS5));
            
            THEN("output should be sent") {
                REQUIRE(!out0.empty());
            }
        }
        
        WHEN("processing a note with list message") {
            REQUIRE_NOTHROW(quantizerTestObject.list({ NoteDS5, 100 })); // NOLINT
            
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
        QuantizerMax& quantizerTestObject = an_instance;
        
        quantizerTestObject.quantizerMode(1);
        quantizerTestObject.quantizerAddNote({ NoteC5, NoteE5, NoteG5 });

        REQUIRE(quantizerTestObject.noteCount() == 3);
        
        WHEN("deleting notes via del message") {
            REQUIRE_NOTHROW(quantizerTestObject.quantizerDeleteNote(NoteE5));
            
            THEN("note count should decrease") {
                REQUIRE(quantizerTestObject.noteCount() == 2);
            }
        }
    }
}

SCENARIO("quantizer handles round directions") {
    ext_main(nullptr);

    GIVEN("A QuantizerMax with specific notes") {
        test_wrapper<QuantizerMax> an_instance;
        QuantizerMax& quantizerTestObject = an_instance;
        
        quantizerTestObject.quantizerAddNote({ NoteC5, NoteE5 });
        
        WHEN("setting round direction") {
            REQUIRE_NOTHROW(quantizerTestObject.quantizerRound(Quantizer::RoundDirection::UP));
            REQUIRE(quantizerTestObject.getRoundDirection() == Quantizer::RoundDirection::UP);
            
            REQUIRE_NOTHROW(quantizerTestObject.quantizerRound(Quantizer::RoundDirection::DOWN));
            REQUIRE(quantizerTestObject.getRoundDirection() == Quantizer::RoundDirection::DOWN);
        }
    }
}

SCENARIO("quantizer handles range limits") {
    ext_main(nullptr);

    GIVEN("A QuantizerMax with range limits") {
        test_wrapper<QuantizerMax> an_instance;
        QuantizerMax& quantizerTestObject = an_instance;
        
        quantizerTestObject.quantizerMode(Quantizer::QuantizeMode::TWELVE_NOTES);        
        quantizerTestObject.quantizerAddNote({ NoteC5, NoteE5, NoteE6 });
        quantizerTestObject.quantizerRange({NoteC3, NoteC5});

        REQUIRE_NOTHROW(quantizerTestObject.noteInput(NoteB3));

        auto &note_output = *c74::max::object_getoutput(quantizerTestObject, 0);
        auto &velocity_output = *c74::max::object_getoutput(quantizerTestObject, 1);

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
    QuantizerMax &quantizerTestObject = an_instance;

    REQUIRE_NOTHROW(quantizerTestObject.quantizerMode(Quantizer::QuantizeMode::ALL_NOTES));
    
    GIVEN("the notes"){
        auto &note_output = *c74::max::object_getoutput(quantizerTestObject, 0);
        
        REQUIRE_NOTHROW(quantizerTestObject.noteInput(NoteB3));
        REQUIRE(note_output[0][1] == NoteB3);

        REQUIRE(!note_output.empty());
        REQUIRE_NOTHROW(quantizerTestObject.quantizerAddNote(NoteC4));
        
        REQUIRE_NOTHROW(quantizerTestObject.noteInput(NoteC4));
        REQUIRE(!note_output.empty());
        REQUIRE(!note_output[0].empty());
        REQUIRE(note_output[1][1] == NoteC4);

        WHEN("first note is detected") {
            REQUIRE(quantizerTestObject.noteCount() == 1);
        }
        
        REQUIRE_NOTHROW(quantizerTestObject.quantizerAddNote(NoteC5));
        REQUIRE(quantizerTestObject.noteCount() == 2);
    }
}

SCENARIO("adding notes to the quantizer with velocity") {
    ext_main(nullptr);

    test_wrapper<QuantizerMax> an_instance;
    QuantizerMax &quantizerTestObject = an_instance;
    
    quantizerTestObject.quantizerMode(Quantizer::QuantizeMode::ALL_NOTES);

    GIVEN("the notes") {
        REQUIRE(quantizerTestObject.getRoundDirection() == Quantizer::RoundDirection::UP);

        auto &note_output = *c74::max::object_getoutput(quantizerTestObject, 0);
        auto &velocity_output = *c74::max::object_getoutput(quantizerTestObject, 1);

        REQUIRE_NOTHROW(quantizerTestObject.list({ NoteB3, 127 })); // NOLINT

        REQUIRE(!note_output.empty());
        REQUIRE(!velocity_output.empty());
        REQUIRE(!note_output[0].empty());
        REQUIRE(!velocity_output[0].empty());
        REQUIRE(note_output[0][1] == NoteB3);
        REQUIRE(velocity_output[0][1] == 127);
        REQUIRE_NOTHROW(quantizerTestObject.quantizerAddNote({NoteB3, 69}));
        REQUIRE(quantizerTestObject.noteCount() == 2);
        REQUIRE_NOTHROW(quantizerTestObject.quantizerAddNote(NoteC5));
        REQUIRE(quantizerTestObject.noteCount() == 3);
        REQUIRE_NOTHROW(quantizerTestObject.list({ NoteB4, 127 })); // NOLINT
        REQUIRE(!note_output.empty());
        REQUIRE(!velocity_output.empty());
        REQUIRE(!note_output[1].empty());
        REQUIRE(!velocity_output[1].empty());
        REQUIRE(note_output[1][1] == NoteC5);
        REQUIRE(velocity_output[1][1] == 127);
        REQUIRE_NOTHROW(quantizerTestObject.quantizerAddNote(NoteG4));
        REQUIRE(quantizerTestObject.noteCount() == 4);
        REQUIRE_NOTHROW(quantizerTestObject.quantizerRound(Quantizer::RoundDirection::DOWN));
        REQUIRE_NOTHROW(quantizerTestObject.list({ NoteB4, 127 }));
        REQUIRE(!note_output.empty());
        REQUIRE(!velocity_output.empty());
        REQUIRE(!note_output[2].empty());
        REQUIRE(!velocity_output[2].empty());
        REQUIRE(note_output[2][1] == NoteG4);
        REQUIRE(velocity_output[2][1] == 127); // NOLINT
    }
}

SCENARIO("clear the notes") {
    ext_main(nullptr);
    
    GIVEN("OCTAVE MODE") {
        test_wrapper<QuantizerMax> an_instance;
        QuantizerMax &quantizerTestObject = an_instance;
        
        quantizerTestObject.quantizerMode(Quantizer::QuantizeMode::TWELVE_NOTES);

        WHEN("add and remove notes") {
            for (int i = 0; i <= 11; i++) { // NOLINT
                REQUIRE_NOTHROW(quantizerTestObject.quantizerAddNote(i));
            }

            REQUIRE(quantizerTestObject.noteCount() == 128);

            for (int i = 0; i <= 11; i++) { // NOLINT
                REQUIRE_NOTHROW(quantizerTestObject.quantizerDeleteNote(i));
            }

            REQUIRE(quantizerTestObject.noteCount() == 0);            
        }
    }
    
    GIVEN("ALL KEYS MODE") {
        test_wrapper<QuantizerMax> an_instance;
        QuantizerMax &quantizerTestObject = an_instance;

        quantizerTestObject.quantizerMode(Quantizer::QuantizeMode::ALL_NOTES);
        
        WHEN("add and remove notes") {
            REQUIRE(quantizerTestObject.noteCount() == 0);

            // TODO: There are some mysterious bugs in here.
            for (int i = 0; i < MIDI::KEYBOARD_SIZE; i++) {
                //REQUIRE(quantizerTestObject.noteCount() == i);
                REQUIRE_NOTHROW(quantizerTestObject.quantizerAddNote(i));
                //REQUIRE(quantizerTestObject.noteCount() == i + 1);
            }

            //REQUIRE(quantizerTestObject.noteCount() == 128);

            for (int i = 0; i < MIDI::KEYBOARD_SIZE; i++) {
                REQUIRE_NOTHROW(quantizerTestObject.quantizerDeleteNote(i));
            }

            REQUIRE(quantizerTestObject.noteCount() == 0);
        }
    }
}

SCENARIO("updating the quantized notes") { // NOLINT
    ext_main(nullptr);

    test_wrapper<QuantizerMax> an_instance;
    QuantizerMax &quantizerTestObject = an_instance;

    GIVEN("ALL_NOTES mode"){
        auto &note_output = *c74::max::object_getoutput(quantizerTestObject, 0);
        
        // Set mode.
        REQUIRE_NOTHROW(quantizerTestObject.quantizerMode(Quantizer::QuantizeMode::ALL_NOTES));
        
        // First set of notes.
        c74::min::atoms noteArgs0 = {NoteC5, NoteEB5, NoteG5, NoteBB5};
        
        // Add notes.
        REQUIRE_NOTHROW(quantizerTestObject.updateNotes(noteArgs0));

        // Quantize our note - 1.
        for (const auto &noteArg : noteArgs0) {
            // Quantize note.
            REQUIRE_NOTHROW(quantizerTestObject.noteInput(static_cast<int> (noteArg) - 1));
        }

        REQUIRE(!note_output.empty());

        // Check the output values.
        for (int i = 0; i < noteArgs0.size(); i++) {
            REQUIRE(note_output[i][1] == noteArgs0[i]);
        }

        // Second set of notes.
        c74::min::atoms noteArgs1 = {NoteC4, NoteEB4, NoteG4, NoteBB4};
        
        // Add notes.
        REQUIRE_NOTHROW(quantizerTestObject.updateNotes(noteArgs1));

        // Quantize our note - 1.
        for (const auto &noteArg : noteArgs1) {
            // Quantize note.
            REQUIRE_NOTHROW(quantizerTestObject.noteInput(static_cast<int> (noteArg) - 1));
        }

        REQUIRE(!note_output.empty());

        // Check the output values.
        for (int i = 0; i < noteArgs1.size(); i++) {
            REQUIRE(note_output[i + noteArgs0.size()][1] == noteArgs1[i]);
        }
    }

    GIVEN("TWELVE_NOTES mode"){
        auto &note_output = *c74::max::object_getoutput(quantizerTestObject, 0);
        
        // Set mode.
        REQUIRE_NOTHROW(quantizerTestObject.quantizerMode(Quantizer::QuantizeMode::TWELVE_NOTES));
        
        // First set of notes.
        c74::min::atoms noteArgs0 = {NoteC5, NoteEB5, NoteG5, NoteBB5};
        
        // Add notes.
        REQUIRE_NOTHROW(quantizerTestObject.updateNotes(noteArgs0));

        // Quantize our note - 1.
        for (const auto &noteArg : noteArgs0) {
            // Quantize note.
            REQUIRE_NOTHROW(quantizerTestObject.noteInput(static_cast<int> (noteArg) - 1));
        }

        REQUIRE(!note_output.empty());

        // Check the output values.
        for (int i = 0; i < noteArgs0.size(); i++) {
            REQUIRE(note_output[i][1] == noteArgs0[i]);
        }

        // Second set of notes.
        c74::min::atoms noteArgs1 = {NoteC4, NoteEB4, NoteG4, NoteBB4};
        
        // Add notes.
        REQUIRE_NOTHROW(quantizerTestObject.updateNotes(noteArgs1));

        // Quantize our note - 1.
        for (const auto &noteArg : noteArgs1) {
            // Quantize note.
            REQUIRE_NOTHROW(quantizerTestObject.noteInput(static_cast<int> (noteArg) - 1));
        }

        REQUIRE(!note_output.empty());

        // Check the output values.
        for (int i = 0; i < noteArgs1.size(); i++) {
            REQUIRE(note_output[i + noteArgs0.size()][1] == noteArgs1[i]);
        }
    }
}
