/// @file       jb.CD4024_test.cpp
///	@ingroup 	jb.cmos
///	@copyright	Copyright 2025 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License found in the License.md file.

#include "c74_min_unittest.h"
#include "jb.CD4024.cpp"

SCENARIO("object produces correct output") {
    ext_main(nullptr);

    GIVEN("An instance of our object") {
        test_wrapper<CD4024_MAX> an_instance;
        CD4024_MAX&              my_object = an_instance;

        WHEN("a 'bang' is received") {
            my_object.bang();

            THEN("a 'bang' is received") {
                REQUIRE(&my_object != nullptr);
            }
            
            THEN("our greeting is produced at the outlet") {
                auto& output_0 = *c74::max::object_getoutput(my_object, 0);
                REQUIRE(output_0.size() == 0);
                //REQUIRE((output_0[0].size() == 1));
                //REQUIRE((output_0[0][0] == int(0)));

                auto& output_1 = *c74::max::object_getoutput(my_object, 1);
                REQUIRE((output_1.size() == 0));
                auto& output_2 = *c74::max::object_getoutput(my_object, 2);
                REQUIRE((output_2.size() == 0));
                auto& output_3 = *c74::max::object_getoutput(my_object, 3);
                REQUIRE((output_3.size() == 0));
                auto& output_4 = *c74::max::object_getoutput(my_object, 4);
                REQUIRE((output_4.size() == 0));
                auto& output_5 = *c74::max::object_getoutput(my_object, 5);
                REQUIRE((output_5.size() == 0));
                auto& output_6 = *c74::max::object_getoutput(my_object, 6);
                REQUIRE((output_6.size() == 0));
            }
        }

        THEN("a 'bang' is received") {
            my_object.bang();
            THEN("our greeting is produced at the outlet") {
                REQUIRE(&my_object != nullptr);
            }

            THEN("our greeting is produced at the outlet") {
                auto& output_0 = *c74::max::object_getoutput(my_object, 0);
                REQUIRE((output_0.size() == 0));
                auto& output_1 = *c74::max::object_getoutput(my_object, 1);
                REQUIRE((output_1.size() == 0));
                auto& output_2 = *c74::max::object_getoutput(my_object, 2);
                REQUIRE((output_2.size() == 0));
                auto& output_3 = *c74::max::object_getoutput(my_object, 3);
                REQUIRE((output_3.size() == 0));
                auto& output_4 = *c74::max::object_getoutput(my_object, 4);
                REQUIRE((output_4.size() == 0));
                auto& output_5 = *c74::max::object_getoutput(my_object, 5);
                REQUIRE((output_5.size() == 0));
                auto& output_6 = *c74::max::object_getoutput(my_object, 6);
                REQUIRE((output_6.size() == 0));
            }
        }

        WHEN("a 'bang' is received") {
            THEN("our greeting is produced at the outlet") {
                REQUIRE((my_object.find_bit(0) == 0));
            }
        }
    }
}
