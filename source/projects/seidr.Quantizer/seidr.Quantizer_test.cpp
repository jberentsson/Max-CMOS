/// @file       seidr.Quantizer_test.cpp
///	@ingroup 	seidr
///	@copyright	Copyright 2025 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License
///             found in the License.md file.

#include "seidr.Quantizer.cpp" // NOLINT
#include "seidr.Quantizer.hpp"
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

SCENARIO("asdasda1") {
    ext_main(nullptr);

    GIVEN("adasd2"){
        test_wrapper<QuantizerMax> an_instance;
        QuantizerMax &myObject = an_instance;

        c74::min::atoms args = {48}; // NOLINT
        REQUIRE_NOTHROW(myObject.anything(48));

        auto &out = *c74::max::object_getoutput(myObject, 0);
       
        REQUIRE(out.empty());
        REQUIRE(myObject.addNote(48) == 0);
        
        REQUIRE_NOTHROW(myObject.anything(48));
        REQUIRE(!out.empty());
        REQUIRE(!out[0].empty());
        REQUIRE(out[0][1] == 48);

        WHEN("asdad2") {
            REQUIRE(true);
        }
    }
}
