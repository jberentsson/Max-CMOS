#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../Keyboard.cpp"

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
    
    SUBCASE("Default range") {
        CHECK(keyboard.low() == 0);
        CHECK(keyboard.high() == 127);
    }
}

TEST_CASE("Keyboard behavior") {
    KeyboardTest keyboard;
    
    SUBCASE("Default range values") {
        CHECK(keyboard.low() == 0);
        CHECK(keyboard.high() == 127);
    }
    
    SUBCASE("Valid range setting") {
        CHECK(keyboard.setRandomRange(3, 4) == 0);
        CHECK(keyboard.low() == 3);
        CHECK(keyboard.high() == 4);
    }
    
    SUBCASE("Invalid range - low > high") {
        keyboard.setRandomRange(3, 4);
        
        CHECK(keyboard.setRandomRange(5, 1) == -1);
        CHECK(keyboard.low() == 3);
        CHECK(keyboard.high() == 4);
    }
    
    SUBCASE("Invalid range - low out of bounds") {
        keyboard.setRandomRange(3, 4);
        
        CHECK(keyboard.setRandomRange(-1, 127) == -2);
        CHECK(keyboard.low() == 3);
        CHECK(keyboard.high() == 4);
    }
    
    SUBCASE("Invalid range - high out of bounds") {
        keyboard.setRandomRange(3, 4);
        
        CHECK(keyboard.setRandomRange(0, 128) == -3);
        CHECK(keyboard.low() == 3);
        CHECK(keyboard.high() == 4);
    }
}

TEST_CASE("Keyboard note generation") {
    KeyboardTest keyboard;
    
    SUBCASE("Note generation within range") {
        keyboard.setRandomRange(48, 96);
        
        std::vector<ActiveNote*> notes = keyboard.note(48, 127);
        REQUIRE(notes.size() > 0);
        CHECK(notes[0]->pitch() >= 48);
        CHECK(notes[0]->pitch() <= 96);
    }
    
    SUBCASE("Multiple note generations") {
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
    
    SUBCASE("Single note range") {
        CHECK(keyboard.setRandomRange(60, 60) == 0);
        std::vector<ActiveNote*> notes = keyboard.note(60, 100);
        REQUIRE(notes.size() > 0);
        CHECK(notes[0]->pitch() == 60);
    }
    
    SUBCASE("Full MIDI range") {
        CHECK(keyboard.setRandomRange(0, 127) == 0);
        std::vector<ActiveNote*> notes = keyboard.note(60, 100);
        REQUIRE(notes.size() > 0);
        CHECK(notes[0]->pitch() >= 0);
        CHECK(notes[0]->pitch() <= 127);
    }
}
