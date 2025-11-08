/// @file       jb.CD4017_test.cpp
///	@ingroup 	jb.cmos
///	@copyright	Copyright 2022 The Min-DevKit Authors. All rights reserved.
///	@license	Use of this source code is governed by the MIT License found in the License.md file.

#include <catch2/catch_all.hpp>
#include "../jb.CD4017.cpp"

SCENARIO("CD4017 object creation") {
    ext_main(nullptr);
    auto* my_object = new CD4017;

    GIVEN("An instance of our object") {
        WHEN("A newly created object") {
            THEN("A newly created object") {
                REQUIRE(&my_object != nullptr);
            }
        }
    }
}

SCENARIO("CD4017 bang processing") {
    ext_main(nullptr);
    auto* my_object = new CD4017;

    GIVEN("An instance of our object") {
        WHEN("A newly created object") {
            THEN("A newly created object") {
                REQUIRE_NOTHROW(my_object->bang());
            }
        }
    }
}

SCENARIO("CD4017 multiple bangs") {
    ext_main(nullptr);
    auto* my_object = new CD4017;

    GIVEN("An instance of our object") {
        WHEN("A newly created object") {
            THEN("A newly created object") {
                for (int i = 0; i < 10; i++) {
                    REQUIRE_NOTHROW(my_object->bang());
                }
            }
        }
    }
}
