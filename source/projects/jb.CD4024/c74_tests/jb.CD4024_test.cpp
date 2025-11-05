/// @file       jb.CD4024_test.cpp
///	@ingroup 	jb.cmos
///	@copyright	Copyright 2018 The Min-DevKit Authors. All rights reserved.
///	@license	Use of this source code is governed by the MIT License found in the License.md file.

// Prevent conflicts with other frameworks
//#define CATCH_CONFIG_NO_WINDOWS_SEH
//#define CATCH_CONFIG_NO_POSIX_SIGNALS
//#define CATCH_CONFIG_FAST_COMPILE

#include "c74_max.h"
#include "c74_min.h"
#include "c74_min_unittest.h"
#include <catch.hpp>

#include "../jb.CD4024.cpp"    // need the source of our object so that we can access it

using namespace c74::max;

TEST_CASE("object produces correct output") {
    ext_main(nullptr);    // every unit test must call ext_main() once to configure the class

    GIVEN("An instance of our object") {

        c74::min::test_wrapper<CD4024> an_instance;
        CD4024&              my_object = an_instance;

        // now proceed to testing various sequences of events
        WHEN("a 'bang' is received") {
            my_object.bang();

            REQUIRE(&my_object != nullptr);

            THEN("our greeting is produced at the outlet") {
                //auto& output_0 = *object_getoutput(my_object, 0);
                //REQUIRE((output_0.size() == 1));
                //auto& output_1 = *object_getoutput(my_object, 1);
                //REQUIRE((output_1.size() == 1));
                //auto& output_2 = *object_getoutput(my_object, 2);
                //REQUIRE((output_2.size() == 1));
                //auto& output_3 = *object_getoutput(my_object, 3);
                //REQUIRE((output_3.size() == 1));
                //auto& output_4 = *object_getoutput(my_object, 4);
                //REQUIRE((output_4.size() == 1));
                //auto& output_5 = *object_getoutput(my_object, 5);
                //REQUIRE((output_5.size() == 1));
                //auto& output_6 = *object_getoutput(my_object, 6);
                //REQUIRE((output_6.size() == 1));
                //REQUIRE((symbol(output_6[0][0]) != symbol('bang')));
            }
        }

        // now proceed to testing various sequences of events
        WHEN("a 'bang' is received") {
            my_object.bang();

            REQUIRE(&my_object != nullptr);

            // TODO: Check out how this works.
            THEN("our greeting is produced at the outlet") {
                //auto& output_0 = *object_getoutput(my_object, 0);
                //REQUIRE((output_0.size() == 1));
                //auto& output_1 = *object_getoutput(my_object, 1);
                //REQUIRE((output_1.size() == 1));
                //auto& output_2 = *object_getoutput(my_object, 2);
                //REQUIRE((output_2.size() == 1));
                //auto& output_3 = *object_getoutput(my_object, 3);
                //REQUIRE((output_3.size() == 1));
                //auto& output_4 = *object_getoutput(my_object, 4);
                //REQUIRE((output_4.size() == 1));
                //auto& output_5 = *object_getoutput(my_object, 5);
                //REQUIRE((output_5.size() == 1));
                //auto& output_6 = *object_getoutput(my_object, 6);
                //REQUIRE((output_6.size() == 1));
                //REQUIRE((symbol(output_6[0][0]) != symbol('bang')));
            }
        }

    // now proceed to testing various sequences of events
        WHEN("a 'bang' is received") {
            THEN("our greeting is produced at the outlet") {
                REQUIRE((my_object.find_bit(0) == 1));
            }
        }
    }
}
