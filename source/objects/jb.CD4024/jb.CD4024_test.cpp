/// @file
///	@ingroup 	minexamples
///	@copyright	Copyright 2018 The Min-DevKit Authors. All rights reserved.
///	@license	Use of this source code is governed by the MIT License found in the License.md file.

#include "c74_min_unittest.h"     // required unit test header
#include "jb.CD4024.cpp"    // need the source of our object so that we can access it

// Unit tests are written using the Catch framework as described at
// https://github.com/philsquared/Catch/blob/master/docs/tutorial.md

SCENARIO("object produces correct output") {
    ext_main(nullptr);    // every unit test must call ext_main() once to configure the class

    GIVEN("An instance of our object") {

        test_wrapper<CD4024> an_instance;
        CD4024&              my_object = an_instance;

        // now proceed to testing various sequences of events
        WHEN("a 'bang' is received") {
            my_object.bang();
            THEN("our greeting is produced at the outlet") {
                auto& output_0 = *c74::max::object_getoutput(my_object, 0);
                REQUIRE((output_0.size() == 1));
                auto& output_1 = *c74::max::object_getoutput(my_object, 1);
                REQUIRE((output_1.size() == 1));
                auto& output_2 = *c74::max::object_getoutput(my_object, 2);
                REQUIRE((output_2.size() == 1));
                auto& output_3 = *c74::max::object_getoutput(my_object, 3);
                REQUIRE((output_3.size() == 1));
                auto& output_4 = *c74::max::object_getoutput(my_object, 4);
                REQUIRE((output_4.size() == 1));
                auto& output_5 = *c74::max::object_getoutput(my_object, 5);
                REQUIRE((output_5.size() == 1));
                auto& output_6 = *c74::max::object_getoutput(my_object, 6);
                REQUIRE((output_6.size() == 1));
                //REQUIRE((symbol(output_6[0][0]) != symbol('bang')));
            }
        }

        // now proceed to testing various sequences of events
        WHEN("a 'bang' is received") {
            my_object.bang();
            my_object.bang();
            THEN("our greeting is produced at the outlet") {
                auto& output_0 = *c74::max::object_getoutput(my_object, 0);
                REQUIRE((output_0.size() == 1));
                auto& output_1 = *c74::max::object_getoutput(my_object, 1);
                REQUIRE((output_1.size() == 1));
                auto& output_2 = *c74::max::object_getoutput(my_object, 2);
                REQUIRE((output_2.size() == 1));
                auto& output_3 = *c74::max::object_getoutput(my_object, 3);
                REQUIRE((output_3.size() == 1));
                auto& output_4 = *c74::max::object_getoutput(my_object, 4);
                REQUIRE((output_4.size() == 1));
                auto& output_5 = *c74::max::object_getoutput(my_object, 5);
                REQUIRE((output_5.size() == 1));
                auto& output_6 = *c74::max::object_getoutput(my_object, 6);
                REQUIRE((output_6.size() == 1));
                REQUIRE((symbol(output_6[0][0]) != symbol('bang')));
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
