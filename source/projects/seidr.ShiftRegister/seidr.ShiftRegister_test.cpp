/// @file       seidr.ShiftRegister_test.cpp
///	@ingroup 	jb
///	@copyright	Copyright 2025 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License found in the License.md file.

#include "c74_min_unittest.h"
#include "seidr.ShiftRegister.hpp"
#include "seidr.ShiftRegister.cpp"

using namespace c74::max;

SCENARIO("create an instance"){
    ShiftRegister_MAX sr = ShiftRegister_MAX();

    REQUIRE(true);

    WHEN("all bits are zero"){
        for(int i = 0; i < sr.size(); i++){
            REQUIRE(sr.get(i) == 0);
        }
    }

    WHEN("set every other bit"){
        for(int i = 0; i < sr.size(); i++){
            sr.data_input(i % 1);
            sr.step();
            REQUIRE(sr.data_through() == 0);
        }
    }

    WHEN("every other bit is set"){
        for(int i = 0; i < sr.size(); i++){
            REQUIRE(sr.get(i) == i % 1);
        }
    }
}
