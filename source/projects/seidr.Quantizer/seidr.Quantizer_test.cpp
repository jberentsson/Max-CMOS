/// @file       seidr.Quantizer_test.cpp
///	@ingroup 	seidr
///	@copyright	Copyright 2025 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License
///             found in the License.md file.

#include "seidr.Quantizer.cpp" // NOLINT
#include "seidr.Quantizer.hpp"
#include <c74_min_unittest.h>

using namespace c74;
using namespace MIDI::Notes;

using Inlets = QuantizerMax::Inlets;
using QuantizeMode = QuantizerMax::QuantizeMode;
using RoundDirection = QuantizerMax::RoundDirection;

SCENARIO("object can be created") {
    ext_main(nullptr);

    GIVEN("A QuantizerMax instance") {
        min::test_wrapper<QuantizerMax> an_instance;
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
        min::test_wrapper<QuantizerMax> an_instance;
        QuantizerMax& quantizerTestObject = an_instance;
        REQUIRE_NOTHROW(quantizerTestObject.quantizerMode(QuantizeMode::ALL_NOTES, 1));
        
        WHEN("adding notes via addNote method") {
            REQUIRE_NOTHROW(quantizerTestObject.quantizerAddNote(NoteC4, 1));
            REQUIRE(quantizerTestObject.noteCount() == 1);
            
            REQUIRE_NOTHROW(quantizerTestObject.quantizerAddNote(NoteC5, 1));
            REQUIRE(quantizerTestObject.noteCount() == 2);
        }
        
        WHEN("adding notes via add message") {
            REQUIRE_NOTHROW(quantizerTestObject.quantizerAddNote({ NoteC4, NoteE4, NoteG4 }, 1));
            REQUIRE(quantizerTestObject.noteCount() == 3);
        }
    }
}

SCENARIO("quantizer processes notes correctly") {
    ext_main(nullptr);

    GIVEN("A QuantizerMax with C major scale") {
        min::test_wrapper<QuantizerMax> an_instance;
        QuantizerMax& quantizerTestObject = an_instance;

        // Set the quantizer mode to ALL_NOTES.
        REQUIRE_NOTHROW(quantizerTestObject.quantizerMode(QuantizeMode::ALL_NOTES));
        
        // Add C major notes.
        REQUIRE_NOTHROW(quantizerTestObject.quantizerAddNote({ NoteC5, NoteD5, NoteE5, NoteF5, NoteG5, NoteA5, NoteB5 }));
        
        auto& out0 = *max::object_getoutput(quantizerTestObject, 0);
        auto& out1 = *max::object_getoutput(quantizerTestObject, 1);
        
        WHEN("processing a note with integer message") {
            // D#5 should quantize to E5.
            REQUIRE_NOTHROW(quantizerTestObject.list({NoteDS5, 75}));
            
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
        min::test_wrapper<QuantizerMax> an_instance;
        QuantizerMax& quantizerTestObject = an_instance;
        
        quantizerTestObject.quantizerMode(1, Inlets::ARGS);
        quantizerTestObject.quantizerAddNote({ NoteC5, NoteE5, NoteG5 }, Inlets::ARGS);

        REQUIRE(quantizerTestObject.noteCount() == 3);
        
        WHEN("deleting notes via del message") {
            REQUIRE_NOTHROW(quantizerTestObject.quantizerDeleteNote(NoteE5, Inlets::ARGS));
            
            THEN("note count should decrease") {
                REQUIRE(quantizerTestObject.noteCount() == 2);
            }
        }
    }
}

SCENARIO("quantizer handles round directions") {
    ext_main(nullptr);

    GIVEN("A QuantizerMax with specific notes") {
        min::test_wrapper<QuantizerMax> an_instance;
        QuantizerMax& quantizerTestObject = an_instance;
        
        quantizerTestObject.quantizerAddNote({ NoteC5, NoteE5 }, Inlets::ARGS);
        
        WHEN("setting round direction") {
            REQUIRE_NOTHROW(quantizerTestObject.quantizerRound(RoundDirection::UP, Inlets::ARGS));
            REQUIRE(quantizerTestObject.getRoundDirection() == RoundDirection::UP);
            
            REQUIRE_NOTHROW(quantizerTestObject.quantizerRound(RoundDirection::DOWN, Inlets::ARGS));
            REQUIRE(quantizerTestObject.getRoundDirection() == RoundDirection::DOWN);
        }
    }
}

SCENARIO("quantizer handles range limits") {
    ext_main(nullptr);

    GIVEN("A QuantizerMax with range limits") {
        min::test_wrapper<QuantizerMax> an_instance;
        QuantizerMax& quantizerTestObject = an_instance;
        
        quantizerTestObject.quantizerMode(QuantizeMode::TWELVE_NOTES);        
        quantizerTestObject.quantizerAddNote({ NoteC5, NoteE5, NoteE6 });
        quantizerTestObject.quantizerRange({NoteC3, NoteC5});

        REQUIRE_NOTHROW(quantizerTestObject.list({ NoteB3, 63 }));

        auto &note_output = *max::object_getoutput(quantizerTestObject, 0);
        auto &velocity_output = *max::object_getoutput(quantizerTestObject, 1);

        REQUIRE(!note_output.empty());
        REQUIRE(!velocity_output.empty());
        REQUIRE(!note_output[0].empty());

        REQUIRE((int) note_output[0][1] >= NoteC3);
        REQUIRE((int) note_output[0][1] <= NoteC5);
    }
}

SCENARIO("adding notes to the quantizer") {
    ext_main(nullptr);

    min::test_wrapper<QuantizerMax> an_instance;
    QuantizerMax &quantizerTestObject = an_instance;

    REQUIRE_NOTHROW(quantizerTestObject.quantizerMode(QuantizeMode::ALL_NOTES, Inlets::ARGS));
    
    GIVEN("the notes"){
        auto &note_output = *max::object_getoutput(quantizerTestObject, 0);
        
        REQUIRE_NOTHROW(quantizerTestObject.list({ NoteB3, 50 }, Inlets::NOTE));
        REQUIRE(note_output[0][1] == NoteB3);

        REQUIRE(!note_output.empty());
        REQUIRE_NOTHROW(quantizerTestObject.quantizerAddNote(NoteC4, Inlets::ARGS));
        
        REQUIRE_NOTHROW(quantizerTestObject.list({ NoteC4, 75 }, Inlets::NOTE));
        REQUIRE(!note_output.empty());
        REQUIRE(!note_output[0].empty());
        REQUIRE(note_output[1][1] == NoteC4);

        WHEN("first note is detected") {
            REQUIRE(quantizerTestObject.noteCount() == 1);
        }
        
        REQUIRE_NOTHROW(quantizerTestObject.quantizerAddNote(NoteC5, Inlets::ARGS));
        REQUIRE(quantizerTestObject.noteCount() == 2);
    }
}

SCENARIO("adding notes to the quantizer with velocity") {
    ext_main(nullptr);

    min::test_wrapper<QuantizerMax> an_instance;
    QuantizerMax &quantizerTestObject = an_instance;
    
    quantizerTestObject.quantizerMode(QuantizeMode::ALL_NOTES, Inlets::ARGS);

    GIVEN("the notes") {
        REQUIRE(quantizerTestObject.getRoundDirection() == RoundDirection::UP);

        auto &note_output = *max::object_getoutput(quantizerTestObject, 0);
        auto &velocity_output = *max::object_getoutput(quantizerTestObject, 1);

        REQUIRE_NOTHROW(quantizerTestObject.list({ NoteB3, 127 }, Inlets::NOTE)); // NOLINT

        REQUIRE(!note_output.empty());
        REQUIRE(!velocity_output.empty());
        REQUIRE(!note_output[0].empty());
        REQUIRE(!velocity_output[0].empty());
        REQUIRE(note_output[0][1] == NoteB3);
        REQUIRE(velocity_output[0][1] == 127);
        REQUIRE_NOTHROW(quantizerTestObject.quantizerAddNote({NoteB3, 69}, Inlets::ARGS));
        REQUIRE(quantizerTestObject.noteCount() == 2);
        REQUIRE_NOTHROW(quantizerTestObject.quantizerAddNote(NoteC5, Inlets::ARGS));
        REQUIRE(quantizerTestObject.noteCount() == 3);
        REQUIRE_NOTHROW(quantizerTestObject.list({ NoteB4, 127 }, Inlets::NOTE)); // NOLINT
        REQUIRE(!note_output.empty());
        REQUIRE(!velocity_output.empty());
        REQUIRE(!note_output[1].empty());
        REQUIRE(!velocity_output[1].empty());
        REQUIRE(note_output[1][1] == NoteC5);
        REQUIRE(velocity_output[1][1] == 127);
        REQUIRE_NOTHROW(quantizerTestObject.quantizerAddNote(NoteG4, Inlets::ARGS));
        REQUIRE(quantizerTestObject.noteCount() == 4);
        REQUIRE_NOTHROW(quantizerTestObject.quantizerRound(RoundDirection::DOWN, Inlets::ARGS));
        REQUIRE_NOTHROW(quantizerTestObject.list({ NoteB4, 127 }, Inlets::NOTE));
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
        min::test_wrapper<QuantizerMax> an_instance;
        QuantizerMax &quantizerTestObject = an_instance;
        
        quantizerTestObject.quantizerMode(QuantizeMode::TWELVE_NOTES, Inlets::ARGS);

        WHEN("add and remove notes") {
            for (int i = 0; i <= 11; i++) { // NOLINT
                REQUIRE_NOTHROW(quantizerTestObject.quantizerAddNote(i, Inlets::ARGS));
            }

            REQUIRE(quantizerTestObject.noteCount() == 128);

            for (int i = 0; i <= 11; i++) { // NOLINT
                REQUIRE_NOTHROW(quantizerTestObject.quantizerDeleteNote(i, Inlets::ARGS));
            }

            REQUIRE(quantizerTestObject.noteCount() == 0);            
        }
    }
    
    GIVEN("ALL KEYS MODE") {
        min::test_wrapper<QuantizerMax> an_instance;
        QuantizerMax &quantizerTestObject = an_instance;

        quantizerTestObject.quantizerMode(QuantizeMode::ALL_NOTES, Inlets::ARGS);
        
        WHEN("add and remove notes") {
            REQUIRE(quantizerTestObject.noteCount() == 0);

            for (int i = 0; i < MIDI::KEYBOARD_SIZE; i++) {
                REQUIRE(quantizerTestObject.noteCount() == i);
                REQUIRE_NOTHROW(quantizerTestObject.quantizerAddNote(i, Inlets::ARGS));
                REQUIRE(quantizerTestObject.noteCount() == i + 1);
            }

            REQUIRE(quantizerTestObject.noteCount() == 128);

            for (int i = 0; i < MIDI::KEYBOARD_SIZE; i++) {
                REQUIRE_NOTHROW(quantizerTestObject.quantizerDeleteNote(i, Inlets::ARGS));
            }

            REQUIRE(quantizerTestObject.noteCount() == 0);
        }
    }
}

SCENARIO("updating the quantized notes") { // NOLINT
    ext_main(nullptr);

    min::test_wrapper<QuantizerMax> an_instance;
    QuantizerMax &quantizerTestObject = an_instance;

    GIVEN("ALL_NOTES mode"){
        auto &note_output = *max::object_getoutput(quantizerTestObject, 0);
        
        // Set mode.
        REQUIRE_NOTHROW(quantizerTestObject.quantizerMode(QuantizeMode::ALL_NOTES, Inlets::ARGS));
        
        // First set of notes.
        min::atoms noteArgs0 = { NoteC5, NoteEB5, NoteG5, NoteBB5 };
        
        // Add notes.
        REQUIRE_NOTHROW(quantizerTestObject.updateNotes(noteArgs0, Inlets::ARGS));

        // Quantize our note - 1.
        for (const auto &noteArg : noteArgs0) {
            // Quantize note.
            REQUIRE_NOTHROW(quantizerTestObject.list({ static_cast<int> (noteArg) - 1, 42}));
        }

        REQUIRE(!note_output.empty());

        // Check the output values.
        for (int i = 0; i < noteArgs0.size(); i++) {
            REQUIRE(note_output[i][1] == noteArgs0[i]);
        }

        // Second set of notes.
        min::atoms noteArgs1 = { NoteC4, NoteEB4, NoteG4, NoteBB4 };
        
        // Add notes.
        REQUIRE_NOTHROW(quantizerTestObject.updateNotes(noteArgs1, Inlets::ARGS));

        // Quantize our note - 1.
        for (const auto &noteArg : noteArgs1) {
            // Quantize note.
            REQUIRE_NOTHROW(quantizerTestObject.list({static_cast<int> (noteArg) - 1, 69}, Inlets::NOTE));
        }

        REQUIRE(!note_output.empty());

        // Check the output values.
        for (int i = 0; i < noteArgs1.size(); i++) {
            REQUIRE(note_output[i + noteArgs0.size()][1] == noteArgs1[i]);
        }
    }

    GIVEN("TWELVE_NOTES mode"){
        auto &note_output = *max::object_getoutput(quantizerTestObject, 0);
        
        // Set mode.
        REQUIRE_NOTHROW(quantizerTestObject.quantizerMode(QuantizeMode::TWELVE_NOTES));
        
        // First set of notes.
        min::atoms noteArgs0 = { NoteC5, NoteEB5, NoteG5, NoteBB5 };
        
        // Add notes.
        REQUIRE_NOTHROW(quantizerTestObject.updateNotes(noteArgs0, Inlets::ARGS));

        // Quantize our note - 1.
        for (const auto &noteArg : noteArgs0) {
            // Quantize note.
            REQUIRE_NOTHROW(quantizerTestObject.list({ static_cast<int> (noteArg) - 1, 99 }, Inlets::NOTE));
        }

        REQUIRE(!note_output.empty());

        // Check the output values.
        for (int i = 0; i < noteArgs0.size(); i++) {
            REQUIRE(note_output[i][1] == noteArgs0[i]);
        }

        // Second set of notes.
        min::atoms noteArgs1 = { NoteC4, NoteEB4, NoteG4, NoteBB4 };
        
        // Add notes.
        REQUIRE_NOTHROW(quantizerTestObject.updateNotes(noteArgs1));

        // Quantize our note - 1.
        for (const auto &noteArg : noteArgs1) {
            // Quantize note.
            REQUIRE_NOTHROW(quantizerTestObject.list({static_cast<int> (noteArg) - 1, 69}));
        }

        REQUIRE(!note_output.empty());

        // Check the output values.
        for (int i = 0; i < noteArgs1.size(); i++) {
            REQUIRE(note_output[i + noteArgs0.size()][1] == noteArgs1[i]);
        }
    }
}
