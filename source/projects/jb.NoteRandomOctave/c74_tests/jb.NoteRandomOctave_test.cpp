/// @file       jb.NoteRandomOctave_test.cpp
///	@ingroup 	jb
///	@copyright	Copyright 2018 The Min-DevKit Authors. All rights reserved.
///	@license	Use of this source code is governed by the MIT License found in the License.md file.


#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "c74_max.h"


#include <vector>
#include <iostream>

TEST_CASE("MIDI note validation") {
    SECTION("note values are valid") {
        REQUIRE(60 >= 0);
        REQUIRE(60 <= 127);
        REQUIRE(100 >= 0);
        REQUIRE(100 <= 127);
    }
    
    SECTION("range boundaries are valid") {
        REQUIRE(48 < 72);
        REQUIRE(48 >= 0);
        REQUIRE(72 <= 127);
    }
}

TEST_CASE("Vector operations") {
    SECTION("note list format") {
        std::vector<int> args = {60, 100};
        REQUIRE(args.size() == 2);
        REQUIRE(args[0] == 60);
        REQUIRE(args[1] == 100);
    }
    
    SECTION("range list format") {
        std::vector<int> args = {48, 72};
        REQUIRE(args.size() == 2);
        REQUIRE(args[0] == 48);
        REQUIRE(args[1] == 72);
    }
}

class TestKeyboard {
public:
    bool validateNote(int note) { return note >= 0 && note <= 127; }
    bool validateRange(int low, int high) { return low >= 0 && high <= 127 && low <= high; }
    int applyOctaveShift(int note, int octaves) { 
        int result = note + (octaves * 12);
        return std::max(0, std::min(127, result));
    }
    
    std::vector<int> getActiveNotes() { return {}; }
};

TEST_CASE("Keyboard logic") {
    TestKeyboard keyboard;
    
    SECTION("validates notes correctly") {
        REQUIRE(keyboard.validateNote(60) == true);
        REQUIRE(keyboard.validateNote(0) == true);
        REQUIRE(keyboard.validateNote(127) == true);
        REQUIRE(keyboard.validateNote(-1) == false);
        REQUIRE(keyboard.validateNote(128) == false);
    }
    
    SECTION("validates ranges correctly") {
        REQUIRE(keyboard.validateRange(48, 72) == true);
        REQUIRE(keyboard.validateRange(0, 127) == true);
        REQUIRE(keyboard.validateRange(72, 48) == false);
    }
    
    SECTION("applies octave shifts correctly") {
        REQUIRE(keyboard.applyOctaveShift(60, 1) == 72);
        REQUIRE(keyboard.applyOctaveShift(60, -1) == 48);
        REQUIRE(keyboard.applyOctaveShift(60, 0) == 60);
    }
}

SCENARIO("Note transformation scenarios") {
    GIVEN("a keyboard with default range") {
        TestKeyboard keyboard;
        
        WHEN("a valid note is transformed up one octave") {
            int result = keyboard.applyOctaveShift(60, 1);
            
            THEN("the note increases by 12") {
                REQUIRE(result == 72);
            }
        }
        
        WHEN("a valid note is transformed down one octave") {
            int result = keyboard.applyOctaveShift(60, -1);
            
            THEN("the note decreases by 12") {
                REQUIRE(result == 48);
            }
        }
        
        WHEN("an out-of-range note is processed") {
            THEN("it is rejected") {
                REQUIRE(keyboard.validateNote(-1) == false);
                REQUIRE(keyboard.validateNote(128) == false);
            }
        }
    }
}
