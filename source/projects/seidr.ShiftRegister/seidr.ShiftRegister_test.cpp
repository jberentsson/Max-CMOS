/// @file       seidr.ShiftRegister_test.cpp
///	@ingroup 	seidr
///	@copyright	Copyright 2025 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License
///             found in the License.md file.

#include "seidr.ShiftRegister.cpp" // NOLINT
#include "seidr.ShiftRegister.hpp"
#include <c74_min_unittest.h>

using namespace c74::max;

SCENARIO("create an instance") { // NOLINT
    ext_main(nullptr);
    
    test_wrapper<ShiftRegisterMax> an_instance;
    auto shiftRegister = ShiftRegisterMax();

    REQUIRE(true);

    WHEN("all bits are zero") {
        for (int i = 0; i < shiftRegister.size(); i++) {
            REQUIRE(shiftRegister.get(i) == 0);
        }
    }

    WHEN("set every other bit") {
        for (int i = 0; i < shiftRegister.size(); i++) {
            int answer = i % 1 == 1 ? 1 : 0;
            shiftRegister.dataInput(answer);
            shiftRegister.step();
            REQUIRE(shiftRegister.dataThrough() == 0);
        }
    }

    WHEN("every other bit is set") {
        int answer = 0;
        for (int i = 0; i < shiftRegister.size(); i++) {
            REQUIRE(shiftRegister.get(i) == answer);
            answer = i % 1;
        }
    }
}

