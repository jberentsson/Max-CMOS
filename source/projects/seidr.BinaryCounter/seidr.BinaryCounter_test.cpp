/// @file       seidr.BinaryCounter_test.cpp
///	@ingroup 	jb
///	@copyright	Copyright 2025 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License
/// found in the License.md file.

#include "c74_min_unittest.h"
#include "seidr.BinaryCounter.cpp"

using namespace c74::max;

SCENARIO("object produces correct output") {
    ext_main(nullptr);

    GIVEN("An instance of our object") {
        test_wrapper<BinaryCounter_MAX> an_instance;
        BinaryCounter_MAX &my_object = an_instance;

        WHEN("test the rollover") {
            my_object.bang();
            my_object.max_value();

            // THEN("a 'bang' is received") { REQUIRE(my_object != nullptr); }

            THEN("check counter value") {
                REQUIRE(my_object.counter_value() == 0);
            }

            THEN("our greeting is produced at the outlet") {
                int expected[6][8] = {
                    {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 1},
                    {0, 0, 0, 0, 0, 0, 1, 0}, {0, 0, 0, 0, 0, 0, 1, 1},
                    {0, 0, 0, 0, 0, 1, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0},
                };

                REQUIRE(my_object.counter_value() == 0);

                my_object.bang();

                REQUIRE(my_object.counter_value() == 1);

                my_object.bang();

                REQUIRE(my_object.counter_value() == 2);

                my_object.bang();

                REQUIRE(my_object.counter_value() == 3);

                my_object.bang();

                REQUIRE(my_object.counter_value() == 4);

                my_object.reset();
                my_object.bang();

                REQUIRE(my_object.counter_value() == 0);

                for (int i = 0; i < 5; i++) {
                    for (int j = 0; j < OUTPUT_COUNT; j++) {
                        auto &out = *object_getoutput(my_object, j);
                        REQUIRE(out[0].size() == 2);
                        REQUIRE(out[i][1] == expected[i][j]);
                    }
                }
            }
        }

        WHEN("test the preset function") {
            THEN("our greeting is produced at the outlet") {
                int expected[6][8] = {
                    {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 1},
                    {0, 0, 1, 0, 0, 0, 0, 0}, {0, 0, 1, 0, 0, 0, 0, 1},
                    {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 1},
                };

                my_object.bang();

                REQUIRE(my_object.counter_value() == 0);

                my_object.bang();

                REQUIRE(my_object.counter_value() == 1);

                my_object.set_preset(32);
                my_object.preset();

                REQUIRE(my_object.counter_value() == 31);

                my_object.bang();

                REQUIRE(my_object.counter_value() == 32);

                my_object.bang();

                REQUIRE(my_object.counter_value() == 33);

                my_object.reset();
                my_object.bang();

                REQUIRE(my_object.counter_value() == 0);

                my_object.bang();

                REQUIRE(my_object.counter_value() == 1);

                for (int i = 0; i < 6; i++) {
                    for (int j = 0; j < OUTPUT_COUNT; j++) {
                        auto &out = *object_getoutput(my_object, j);
                        REQUIRE(out[0].size() == 2);
                        REQUIRE(out[i][1] == expected[i][j]);
                    }
                }
            }
        }
    }
}
