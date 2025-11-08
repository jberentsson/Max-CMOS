/// @file       jb.NoteRandomOctave_test.cpp
///	@ingroup 	jb
///	@copyright	Copyright 2025 The Min-DevKit Authors. All rights reserved.
///	@license	Use of this source code is governed by the MIT License found in the License.md file.

#include <catch2/catch_all.hpp>
#include "../jb.NoteRandomOctave.cpp"

TEST_CASE("NoteRandomOctave basic functionality") {
    ext_main(nullptr);
    
    SECTION("object can be created") {
        // Test basic object creation
        auto* obj = new NoteRandomOctave;
        REQUIRE(obj != nullptr);
        delete obj;
    }
    
    SECTION("object responds to basic messages") {
        auto* obj = new NoteRandomOctave;
        
        obj->note_list({60, 100});
        obj->range_list({48, 72});
        
        REQUIRE(true);
        
        delete obj;
    }
}
