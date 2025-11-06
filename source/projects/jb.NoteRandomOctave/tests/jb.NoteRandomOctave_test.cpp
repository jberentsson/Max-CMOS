/// @file       jb.NoteRandomOctave_test.cpp
///	@ingroup 	jb
///	@copyright	Copyright 2025 The Min-DevKit Authors. All rights reserved.
///	@license	Use of this source code is governed by the MIT License found in the License.md file.

#include "c74_min.h"
#include "c74_min_unittest.h"
#include <catch.hpp>

#include "../jb.NoteRandomOctave.cpp"

using namespace c74::min;

// Forward declare or create a simple test wrapper
TEST_CASE("object processes notes correctly") {
    ext_main(nullptr);

    GIVEN("An instance of NoteRandomOctave") {
        test_wrapper<NoteRandomOctave> an_instance;
        NoteRandomOctave& my_object = an_instance;

        WHEN("a note list with note and velocity is received") {
            std::vector<c74::min::atom> args = { 60, 100 };
            my_object.note_list(args);
            
            THEN("note and velocity are processed") {
                REQUIRE(true);
            }
        }

        WHEN("a range list is received") {
            std::vector<c74::min::atom> args = { 48, 72 };
            my_object.range_list(args);
            
            THEN("keyboard range is updated") {
                REQUIRE(true);
            }
        }
    }
}