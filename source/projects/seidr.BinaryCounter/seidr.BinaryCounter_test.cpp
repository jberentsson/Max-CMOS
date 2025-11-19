/// @file       seidr.BinaryCounter_test.cpp
///	@ingroup 	jb
///	@copyright	Copyright 2025 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License
/// found in the License.md file.

#include "c74_min_unittest.h"

#include "seidr.BinaryCounter.hpp"
#include "seidr.BinaryCounter.cpp"

using namespace c74::max;

SCENARIO("object produces correct output") {
    ext_main(nullptr);

    GIVEN("An instance of our object") {
        test_wrapper<BinaryCounterMax> an_instance;
        BinaryCounterMax &my_object = an_instance;

        WHEN("test the rollover") {
            my_object.bang();
            my_object.maxValue();

            // THEN("a 'bang' is received") { REQUIRE(my_object != nullptr); }

            THEN("check counter value") {
                REQUIRE(my_object.counterValue() == 1);
            }

            THEN("our greeting is produced at the outlet") {
                int expected[6][8] = {
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 1},
                    {0, 0, 0, 0, 0, 0, 1, 0},
                    {0, 0, 0, 0, 0, 0, 1, 1},
                    {0, 0, 0, 0, 0, 1, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                };

                REQUIRE(my_object.counterValue() == 1);

                my_object.bang();

                REQUIRE(my_object.counterValue() == 2);

                my_object.bang();

                REQUIRE(my_object.counterValue() == 3);

                my_object.bang();

                REQUIRE(my_object.counterValue() == 4);

                my_object.bang();

                REQUIRE(my_object.counterValue() == 5);

                my_object.reset();
                my_object.bang();

                REQUIRE(my_object.counterValue() == 1);

                for (int i = 0; i < 5; i++) {
                    for (int j = 0; j < my_object.OUTPUT_COUNT; j++) {
                        auto &out = *object_getoutput(my_object, j);
                        REQUIRE(out.size() > 0);
                        //REQUIRE(out[0].size() == 2);
                        //REQUIRE(out[i][1] == expected[i][j]);
                    }
                }
            }
        }

        WHEN("test the preset function") {
            THEN("our greeting is produced at the outlet") {
                int expected[6][8] = {
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 1},
                    {0, 0, 1, 0, 0, 0, 0, 0},
                    {0, 0, 1, 0, 0, 0, 0, 1},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 1},
                };

                my_object.bang();

                REQUIRE(my_object.counterValue() == 1);

                my_object.bang();

                REQUIRE(my_object.counterValue() == 2);

                my_object.setPreset(32);
                my_object.preset();

                REQUIRE(my_object.counterValue() == 31);

                my_object.bang();

                REQUIRE(my_object.counterValue() == 0);

                my_object.bang();

                REQUIRE(my_object.counterValue() == 1);

                my_object.reset();
                my_object.bang();

                REQUIRE(my_object.counterValue() == 1);

                my_object.bang();

                REQUIRE(my_object.counterValue() == 2);

                for (int i = 0; i < 6; i++) {
                    for (int j = 0; j < my_object.OUTPUT_COUNT; j++) {
                        auto &out = *object_getoutput(my_object, j);
                        REQUIRE(out.size() > 0);
                        //REQUIRE(out[0].size() == 0);
                        //REQUIRE(out[i][1] == expected[i][j]);
                    }
                }
            }
        }
    }
}
