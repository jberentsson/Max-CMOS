/// @file       seidr.NCounter_test.cpp
///	@ingroup 	jb
///	@copyright	Copyright 2025 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License
/// found in the License.md file.

#include "c74_min_unittest.h"
#include "seidr.NCounter.hpp"

using namespace c74::max;

SCENARIO("NCounterMax object produces correct output") { // NOLINT 
    ext_main(nullptr);

    GIVEN("An instance of our object") {
        test_wrapper<NCounterMax> an_instance;
        NCounterMax &myObject = an_instance;

        WHEN("test the rollover") {
            THEN("check counter value and outputs") {
                // Expected outputs for each step (which output is active)
                int expected[12][10] = {
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Step 0
                    {0, 1, 0, 0, 0, 0, 0, 0, 0, 0}, // Step 1
                    {0, 0, 1, 0, 0, 0, 0, 0, 0, 0}, // Step 2
                    {0, 0, 0, 1, 0, 0, 0, 0, 0, 0}, // Step 3
                    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0}, // Step 4
                    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0}, // Step 5
                    {0, 0, 0, 0, 0, 0, 1, 0, 0, 0}, // Step 6
                    {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, // Step 7
                    {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, // Step 8
                    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // Step 9
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Step 10 (wrap around)
                    {0, 1, 0, 0, 0, 0, 0, 0, 0, 0}  // Step 11
                };

                // Test initial state
                REQUIRE(myObject.counterValue() == 0);

                myObject.bang();

                // Test stepping through values
                for (int step = 0; step < 10; step++) {
                    // Check current outputs
                    for (int output_index = 0; output_index < 10; output_index++) {
                        auto &out = *object_getoutput(myObject, output_index);
                        REQUIRE(out[step].size() >= 1);

                        if (out[0].size() >= 1) {
                            REQUIRE(out[step][1] == expected[step][output_index]);
                        }
                    }

                    // Step to next value
                    myObject.bang();
                }

                // // Test reset
                myObject.reset();
                REQUIRE(myObject.counterValue() == 0);

                // // Verify outputs after reset
                for (int output_index = 0; output_index < 10; output_index++) {
                    auto &out = *object_getoutput(myObject, output_index);
                    REQUIRE(out.size() >= 1);

                    if (out[0].size() > 0) {
                        REQUIRE(out[0][1] == expected[0][output_index]);
                    }
                }
            }
        }

        WHEN("test the preset function") {
            THEN("check preset behavior") {
                // Expected outputs after preset operations
                int expected[6][10] = {
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Initial
                    {0, 1, 0, 0, 0, 0, 0, 0, 0, 0}, // After 1 bang
                    {0, 0, 0, 0, 0, 0, 1, 0, 0, 0}, // After preset to 6
                    {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, // After bang from preset
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // After reset
                    {0, 1, 0, 0, 0, 0, 0, 0, 0, 0}  // After bang from reset
                };

                // Initial state
                REQUIRE(myObject.counterValue() == 0);
                myObject.bang();
                REQUIRE(myObject.counterValue() == 0);

                // Step once
                myObject.bang();
                REQUIRE(myObject.counterValue() == 1);

                // Set preset and activate it
                myObject.setPreset(6);
                myObject.preset();
                myObject.step();
                REQUIRE(myObject.counterValue() == 6);

                // Step from preset
                myObject.bang();
                REQUIRE(myObject.counterValue() == 7);

                // Reset and step again
                myObject.reset();
                REQUIRE(myObject.counterValue() == 0);

                myObject.bang();
                REQUIRE(myObject.counterValue() == 1);

                // Verify all outputs match expected
                int step = 0;

                for (int output_index = 0; output_index < 10; output_index++) {
                    auto &out = *object_getoutput(myObject, output_index);
                    REQUIRE(out.size() >= 1);

                    if (out[0].size() > 0) {
                        REQUIRE(out[step][1] == expected[step][output_index]);
                    }
                }
            }
        }

        WHEN("test maximum value rollover") {
            THEN("counter should wrap around correctly") {
                // Go to maximum value
                for (int i = 0; i < 10; i++) {
                    myObject.bang();
                }

                REQUIRE(myObject.counterValue() == 9);

                // Next bang should wrap to 0
                myObject.bang();

                REQUIRE(myObject.counterValue() == 0);

                // Verify output 0 is active after wrap
                auto &out0 = *object_getoutput(myObject, 0);
                REQUIRE(out0.size() >= 1);

                if (out0[0].size() > 0) {
                    REQUIRE(out0[0][1] == 1); // Output 0 should be active
                }

                auto &out9 = *object_getoutput(myObject, 9);
                REQUIRE(out9.size() >= 1);

                if (out9[0].size() > 0) {
                    REQUIRE(out9[0][1] == 0); // Output 9 should be inactive
                }
            }
        }
    }
}
