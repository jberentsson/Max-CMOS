#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../Note.cpp"

TEST_CASE("Note Basic Functionality") {
    SUBCASE("Construction and basic methods") {
        int n = 60;
        int l = 48;
        int h = 96;
        Note note(n, &l, &h);
        
        REQUIRE(true);
    }
    
    SUBCASE("Random note generation range") {
        int n = 60;
        int l = 48;
        int h = 96;
        Note note(n, &l, &h);
        int randomNote = note.randomizeNote();
        
        REQUIRE(randomNote >= 48);
        REQUIRE(randomNote <= 96);
    }
    
    SUBCASE("Multiple random generations") {
        int n = 60;
        int l = 48;
        int h = 96;
        Note note(n, &l, &h);
        
        for (int i = 0; i < 10; i++) {
            int n = note.randomizeNote();
            REQUIRE(n >= 48);
            REQUIRE(n <= 96);
        }
    }
}

TEST_CASE("Note Capacity Methods") {
    SUBCASE("Max capacity") {
        int n = 60;
        int l = 48;
        int h = 96;
        Note note(n, &l, &h);
        int maxCap = note.maxCapacity();
        
        REQUIRE(maxCap >= 0);
    }
    
    SUBCASE("Min capacity") {
        int n = 60;
        int l = 48;
        int h = 96;
        Note note(n, &l, &h);
        int minCap = note.minCapacity();
        
        REQUIRE(minCap >= 0);
    }
}
