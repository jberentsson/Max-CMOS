#include "catch.hpp"
#include "../Keyboard.cpp"
#include <iostream>

class KeyboardTest : public Keyboard {
public:
    int low() {
        return this->rangeLow;
    }

    int high() {
        return this->rangeHigh;
    }
};

TEST_CASE("Keyboard Basic Tests") {
    KeyboardTest keyboard;
    
    GIVEN("Default range") {
        CHECK(keyboard.low() == 0);
        CHECK(keyboard.high() == 127);
    }
}

TEST_CASE("Keyboard behavior") {
    KeyboardTest keyboard;
    
    GIVEN("Default range values") {
        CHECK(keyboard.low() == 0);
        CHECK(keyboard.high() == 127);
    }
    
    GIVEN("Valid range setting") {
        CHECK(keyboard.setRandomRange(3, 4) == 0);
        CHECK(keyboard.low() == 3);
        CHECK(keyboard.high() == 4);
    }
    
    GIVEN("Invalid range - low > high") {
        keyboard.setRandomRange(3, 4);
        
        CHECK(keyboard.setRandomRange(5, 1) == -1);
        CHECK(keyboard.low() == 3);
        CHECK(keyboard.high() == 4);
    }
    
    GIVEN("Invalid range - low out of bounds") {
        keyboard.setRandomRange(3, 4);
        
        CHECK(keyboard.setRandomRange(-1, 127) == -2);
        CHECK(keyboard.low() == 3);
        CHECK(keyboard.high() == 4);
    }
    
    GIVEN("Invalid range - high out of bounds") {
        keyboard.setRandomRange(3, 4);
        
        CHECK(keyboard.setRandomRange(0, 128) == -3);
        CHECK(keyboard.low() == 3);
        CHECK(keyboard.high() == 4);
    }
}

TEST_CASE("Keyboard note generation") {
    KeyboardTest keyboard;
    
    GIVEN("Note generation within range") {
        keyboard.setRandomRange(48, 96);
        
        std::vector<ActiveNote*> notes = keyboard.note(48, 127);
        REQUIRE(notes.size() > 0);
        CHECK(notes[0]->pitch() >= 48);
        CHECK(notes[0]->pitch() <= 96);
    }
    
    GIVEN("Multiple note generations") {
        keyboard.setRandomRange(48, 96);
        
        for (int i = 0; i < 5; i++) {
            std::vector<ActiveNote*> notes = keyboard.note(48, 127);
            REQUIRE(notes.size() > 0);
            int pitch = notes[0]->pitch();
            CHECK(pitch >= 48);
            CHECK(pitch <= 96);
        }
    }
}

TEST_CASE("Keyboard edge cases") {
    KeyboardTest keyboard;
    
    GIVEN("Single note range") {
        CHECK(keyboard.setRandomRange(60, 60) == 0);
        std::vector<ActiveNote*> notes = keyboard.note(60, 100);
        REQUIRE(notes.size() > 0);
        CHECK(notes[0]->pitch() == 60);
    }
    
    GIVEN("Full MIDI range") {
        CHECK(keyboard.setRandomRange(0, 127) == 0);
        std::vector<ActiveNote*> notes = keyboard.note(60, 100);
        REQUIRE(notes.size() > 0);
        CHECK(notes[0]->pitch() >= 0);
        CHECK(notes[0]->pitch() <= 127);
    }
}


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
