/// @file       seidr.BinaryCounter_test.cpp
///	@ingroup 	jb
///	@copyright	Copyright 2025 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License
/// found in the License.md file.

#include "c74_min_unittest.h"
#include "seidr.BinaryCounter.cpp" // NOLINT
#include "seidr.BinaryCounter.hpp"

using namespace c74::max;

SCENARIO("object produces correct output") { // NOLINT
    ext_main(nullptr);
    GIVEN("An instance of our object") {
        test_wrapper<BinaryCounterMax> an_instance;
        BinaryCounterMax &myObject = an_instance;

        WHEN("test the rollover") {
            myObject.bang();
            myObject.maxValue();

            // THEN("a 'bang' is received") { REQUIRE(myObject != nullptr); }

            THEN("check counter value") {
                REQUIRE(myObject.counterValue() == 1);
            }

            THEN("our greeting is produced at the outlet") {
                int expected[6][8] = {
                    // NOLINT
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 1},
                    {0, 0, 0, 0, 0, 0, 1, 0},
                    {0, 0, 0, 0, 0, 0, 1, 1},
                    {0, 0, 0, 0, 0, 1, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                };

                REQUIRE(myObject.counterValue() == 1);

                myObject.bang();

                REQUIRE(myObject.counterValue() == 2);

                myObject.bang();

                REQUIRE(myObject.counterValue() == 3);

                myObject.bang();

                REQUIRE(myObject.counterValue() == 4);

                myObject.bang();

                REQUIRE(myObject.counterValue() == 5);

                myObject.reset();
                myObject.bang();

                REQUIRE(myObject.counterValue() == 1);

                for (int i = 0; i < 5; i++) { // NOLINT
                    for (int j = 0; j < BinaryCounterMax::OUTPUT_COUNT; j++) {
                        auto &out = *object_getoutput(myObject, j);
                        REQUIRE(!out.empty());
                        // REQUIRE(out[0].size() == 2);
                        // REQUIRE(out[i][1] == expected[i][j]);
                    }
                }
            }
        }

        WHEN("test the preset function") {
            THEN("our greeting is produced at the outlet") {
                int expected[6][8] = {
                    // NOLINT
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 1},
                    {0, 0, 1, 0, 0, 0, 0, 0},
                    {0, 0, 1, 0, 0, 0, 0, 1},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 1},
                };

                myObject.bang();

                REQUIRE(myObject.counterValue() == 1);

                myObject.bang();

                REQUIRE(myObject.counterValue() == 2);

                myObject.setPreset(32); // NOLINT
                myObject.preset();

                REQUIRE(myObject.counterValue() == 31);

                myObject.bang();

                REQUIRE(myObject.counterValue() == 0);

                myObject.bang();

                REQUIRE(myObject.counterValue() == 1);

                myObject.reset();
                myObject.bang();

                REQUIRE(myObject.counterValue() == 1);

                myObject.bang();

                REQUIRE(myObject.counterValue() == 2);

                for (int i = 0; i < 6; i++) { // NOLINT
                    for (int j = 0; j < BinaryCounterMax::OUTPUT_COUNT; j++) {
                        auto &out = *object_getoutput(myObject, j);
                        REQUIRE(!out.empty());
                        // REQUIRE(out[0].size() == 0);
                        // REQUIRE(out[i][1] == expected[i][j]);
                    }
                }
            }
        }
    }
}
