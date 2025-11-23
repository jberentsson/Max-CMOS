/// @file       seidr.Quantizer_test.cpp
///	@ingroup 	seidr
///	@copyright	Copyright 2025 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License
/// found in the License.md file.

#include "seidr.Quantizer.hpp"
#include "seidr.Quantizer.cpp" // NOLINT
#include <c74_min_unittest.h>

using namespace c74::max;

SCENARIO("create an instance") { // NOLINT
    ext_main(nullptr);
    GIVEN("An instance of our object") {
        test_wrapper<QuantizerMax> an_instance;
        QuantizerMax &myObject = an_instance;
        
        WHEN("something"){
            REQUIRE(true);
        }
    }
}
