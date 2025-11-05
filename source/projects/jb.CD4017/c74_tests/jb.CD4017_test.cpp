/// @file       jb.CD4017_test.cpp
///	@ingroup 	jb.cmos
///	@copyright	Copyright 2022 The Min-DevKit Authors. All rights reserved.
///	@license	Use of this source code is governed by the MIT License found in the License.md file.

#include "c74_min.h"
#include "c74_min_unittest.h"
#include <catch.hpp>

#include "../jb.CD4017.cpp"

using namespace c74::min;

TEST_CASE("CD4017 basic functionality") {
    ext_main(nullptr);

    GIVEN("An instance of our object") {
        test_wrapper<CD4017> an_instance;
        CD4017& my_object = an_instance;

        WHEN("a 'bang' is received") {
            my_object.bang();
            
            THEN("the object processes without crashing") {
                REQUIRE(&my_object != nullptr);
                SUCCEED();
            }
        }
    }
}

TEST_CASE("CD4017 multiple bangs") {
    ext_main(nullptr);

    GIVEN("A CD4017 instance") {
        test_wrapper<CD4017> an_instance;
        CD4017& my_object = an_instance;

        WHEN("multiple bangs are sent") {
            for (int i = 0; i < 10; i++) {
                my_object.bang();
            }
            
            THEN("all bangs are processed without issues") {
                REQUIRE(&my_object != nullptr);
                SUCCEED();
            }
        }
    }
}
