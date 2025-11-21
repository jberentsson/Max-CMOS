/// @file       seidr.ShiftRegister_test.cpp
///	@ingroup 	jb
///	@copyright	Copyright 2025 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License
/// found in the License.md file.

#include "c74_min_unittest.h"
#include "seidr.ShiftRegister.hpp"
#include "seidr.ShiftRegister.cpp" // NOLINT

using namespace c74::max;

SCENARIO("create an instance") { // NOLINT 
    ShiftRegisterMax shiftRegister = ShiftRegisterMax();

    REQUIRE(true);

    WHEN("all bits are zero") {
        for (int i = 0; i < shiftRegister.size(); i++) {
            REQUIRE(shiftRegister.get(i) == 0);
        }
    }

    WHEN("set every other bit") {
        int answer = 1;

        for (int i = 0; i < shiftRegister.size(); i++) {
            shiftRegister.dataInput(answer);
            answer = (int) !(bool) answer;
            shiftRegister.step();
            REQUIRE(shiftRegister.dataThrough() == 0);
        }
    }

    //   WHEN("every other bit is set") {
    //     int answer = 0;
    //     for (int i = 0; i < shiftRegister.size(); i++) {
    //       REQUIRE(shiftRegister.get(i) == answer);
    //       answer = !answer;
    //     }
    //   }
}
