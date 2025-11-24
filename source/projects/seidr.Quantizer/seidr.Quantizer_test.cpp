/// @file       seidr.Quantizer_test.cpp
///	@ingroup 	seidr
///	@copyright	Copyright 2025 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License
///             found in the License.md file.

#include "Utils/MIDI.hpp"
#include "Quantizer/Quantizer.hpp"
#include "seidr.Quantizer.cpp" // NOLINT
#include "seidr.Quantizer.hpp"
#include <c74_min_unittest.h>

using namespace c74::max;

SCENARIO("create an instance") { // NOLINT
    ext_main(nullptr);
    
    GIVEN("An instance of our object") {
        test_wrapper<QuantizerMax> an_instance;
        QuantizerMax &myObject = an_instance;
        
        WHEN("something"){
            REQUIRE(true);
        }
    }
}

SCENARIO("adding notes to the quantizer") {
    ext_main(nullptr);

    test_wrapper<QuantizerMax> an_instance;
    QuantizerMax &myObject = an_instance;
    
    GIVEN("the notes"){

        c74::min::atoms args = {MIDI::Notes::NoteC4}; // NOLINT
        REQUIRE_NOTHROW(myObject.anything(48));

        auto &out = *c74::max::object_getoutput(myObject, 0);
       
        REQUIRE(out.empty());
        REQUIRE(myObject.addNote(MIDI::Notes::NoteC4) == 0);
        
        REQUIRE_NOTHROW(myObject.anything(MIDI::Notes::NoteC4));
        REQUIRE(!out.empty());
        REQUIRE(!out[0].empty());
        REQUIRE(out[0][1] == 48);

        WHEN("first note is detected") {
            REQUIRE(myObject.noteCount() == 1);
        }
        
        REQUIRE(myObject.addNote(MIDI::Notes::NoteC5) == 0);
        REQUIRE(myObject.noteCount() == 2);
    }
}

SCENARIO("adding notes to the quantizer with velocity") {
    ext_main(nullptr);

    test_wrapper<QuantizerMax> an_instance;
    QuantizerMax &myObject = an_instance;
    
    GIVEN("the notes"){

        REQUIRE( myObject.getRoundDirection() == Quantizer::RoundDirection::UP );

        auto &out0 = *c74::max::object_getoutput(myObject, 0);
        auto &out1 = *c74::max::object_getoutput(myObject, 1);
       
        c74::min::atoms args = {MIDI::Notes::NoteC4, 127}; // NOLINT
        REQUIRE_NOTHROW(myObject.anything(args));
       
        REQUIRE(out0.empty());
        REQUIRE(out1.empty());
        REQUIRE(myObject.addNote(MIDI::Notes::NoteC4) == 0);
        REQUIRE_NOTHROW(myObject.anything(args));
        REQUIRE(!out0.empty());
        REQUIRE(!out1.empty());
        REQUIRE(!out0[0].empty());
        REQUIRE(!out1[0].empty());
        REQUIRE(out0[0][1] == 48);
        REQUIRE(out1[0][1] == 127);
        REQUIRE(myObject.noteCount() == 1);

        REQUIRE(myObject.addNote(MIDI::Notes::NoteC5) == 0);
        REQUIRE(myObject.noteCount() == 2);

        c74::min::atoms args2 = {MIDI::Notes::NoteB4, 127}; // NOLINT
        REQUIRE_NOTHROW(myObject.anything(args2));

        REQUIRE(!out0.empty());
        REQUIRE(!out1.empty());
        REQUIRE(!out0[1].empty());
        REQUIRE(!out1[1].empty());
        REQUIRE(out0[1][1] == MIDI::Notes::NoteC5);
        REQUIRE(out1[1][1] == 127);

        REQUIRE(myObject.addNote(MIDI::Notes::NoteG4) == 0);
        REQUIRE(myObject.noteCount() == 3);
    
        REQUIRE_NOTHROW(myObject.setRoundDirection(Quantizer::RoundDirection::DOWN));

        c74::min::atoms args3 = {MIDI::Notes::NoteB4, 127}; // NOLINT
        REQUIRE_NOTHROW(myObject.anything(args3));

        REQUIRE(!out0.empty());
        REQUIRE(!out1.empty());
        REQUIRE(!out0[2].empty());
        REQUIRE(!out1[2].empty());
        REQUIRE(out0[2][1] == MIDI::Notes::NoteG4);
        REQUIRE(out1[2][1] == 127); // NOLINT
    }
}
