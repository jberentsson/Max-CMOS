/// @file       seidr.BinaryCounter_test.cpp
///	@ingroup 	seidr
///	@copyright	Copyright 2025 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License
///             found in the License.md file.

#include "seidr.BinaryCounter.cpp" // NOLINT
#include "seidr.BinaryCounter.hpp"
#include <c74_min_unittest.h>

using namespace c74::max;

SCENARIO("object produces correct output") { // NOLINT
    ext_main(nullptr);
    GIVEN("An instance of our object") {
        test_wrapper<BinaryCounterMax> an_instance;
        BinaryCounterMax &myObject = an_instance;

        WHEN("test the rollover") {
            myObject.bang();
            myObject.maxValue();

            THEN("check counter value") {
                REQUIRE(myObject.counterValue() == 0);
            }

            THEN("our greeting is produced at the outlet") {
                int expected[6][8] = {// NOLINT
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 1},
                    {0, 0, 0, 0, 0, 0, 1, 0},
                    {0, 0, 0, 0, 0, 0, 1, 1},
                    {0, 0, 0, 0, 0, 1, 0, 0},
                };

                REQUIRE(myObject.counterValue() == 0);

                myObject.bang(0);

                REQUIRE(myObject.counterValue() == 1);

                myObject.bang(0);

                REQUIRE(myObject.counterValue() == 2);

                myObject.bang(0);

                REQUIRE(myObject.counterValue() == 3);

                myObject.bang(0);

                REQUIRE(myObject.counterValue() == 4);

                myObject.reset((atoms) {}, 1);
                myObject.bang(0);

                REQUIRE(myObject.counterValue() == 0);

                for (int i = 0; i < 5; i++) { // NOLINT
                    for (int j = 0; j < BinaryCounterMax::OUTPUT_COUNT; j++) {
                        auto &out = *object_getoutput(myObject, j);
                        REQUIRE(!out.empty());
                        REQUIRE(out[i][0] == expected[i][j]);
                    }
                }
            }
        }

        WHEN("test the preset function") {
            THEN("our greeting is produced at the outlet") {
                int expected[][8] = {// NOLINT
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 1},
                    {0, 0, 0, 0, 1, 1, 1, 1},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 1},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 1},
                };

                myObject.max_value(16); // NOLINT

                myObject.bang(0);

                REQUIRE(myObject.counterValue() == 0);

                myObject.bang(0);

                REQUIRE(myObject.counterValue() == 1);

                myObject.preset_msg(14, 1); // NOLINT
                myObject.preset_msg((atoms) {},1);
                myObject.bang(0);

                REQUIRE(myObject.counterValue() == 15);

                myObject.bang(0);

                REQUIRE(myObject.counterValue() == 0);

                myObject.bang(0);

                REQUIRE(myObject.counterValue() == 1);

                myObject.reset((atoms) {}, 1);
                myObject.bang(0);

                REQUIRE(myObject.counterValue() == 0);

                myObject.bang(0);

                REQUIRE(myObject.counterValue() == 1);

                for (int i = 0; i < 7; i++) { // NOLINT
                    for (int j = 0; j < BinaryCounterMax::OUTPUT_COUNT; j++) {
                        auto &out = *object_getoutput(myObject, j);
                        REQUIRE(!out.empty());
                        REQUIRE(out[i][0] == expected[i][j]);
                    }
                }
            }
        }
    }
}

SCENARIO("enable bangs works") { // NOLINT
    ext_main(nullptr);

    GIVEN("An instance of our object") {
        test_wrapper<BinaryCounterMax> an_instance;
        BinaryCounterMax &myObject = an_instance;

        auto &out = *object_getoutput(myObject, 7); // NOLINT

        WHEN("test the bangs") {
            myObject.bangEnable(1);
            
            myObject.bang(0);
            myObject.bang(0);

            REQUIRE(!out.empty());
            REQUIRE(!out[0].empty());
            REQUIRE(out[0][0] == "bang");
            myObject.bangDisable(1);
            myObject.reset(1);
            
            myObject.bang(0);
            REQUIRE(out[1][0] == 0);
            myObject.bang(0);
            REQUIRE(out[2][0] == 1);
        }
    }
}
