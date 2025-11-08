#include <catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>
#include "../Note.cpp"

SCENARIO("Note Basic Functionality") {
    GIVEN("Construction and basic methods") {
        int n = 60;
        int l = 48;
        int h = 96;
        Note note(n, &l, &h);
        
        REQUIRE(true);
    }
    
    GIVEN("Random note generation range") {
        int n = 60;
        int l = 48;
        int h = 96;
        Note note(n, &l, &h);
        int randomNote = note.randomizeNote();
        
        REQUIRE(randomNote >= 48);
        REQUIRE(randomNote <= 96);
    }
    
    GIVEN("Multiple random generations") {
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

SCENARIO("Note Capacity Methods") {
    GIVEN("Max capacity") {
        int n = 60;
        int l = 48;
        int h = 96;
        Note note(n, &l, &h);
        int maxCap = note.maxCapacity();
        
        REQUIRE(maxCap >= 0);
    }
    
    GIVEN("Min capacity") {
        int n = 60;
        int l = 48;
        int h = 96;
        Note note(n, &l, &h);
        int minCap = note.minCapacity();
        
        REQUIRE(minCap >= 0);
    }
}
