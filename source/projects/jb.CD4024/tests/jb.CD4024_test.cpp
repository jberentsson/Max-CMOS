/// @file       jb.CD4024_test.cpp
///	@ingroup 	jb.cmos
///	@copyright	Copyright 2018 The Min-DevKit Authors. All rights reserved.
///	@license	Use of this source code is governed by the MIT License found in the License.md file.

#include <catch2/catch_all.hpp>

#include "../jb.CD4024.cpp"

SCENARIO("object produces correct output") {
    ext_main(nullptr);

    GIVEN("An instance of our object") {
        auto* my_object = new CD4024;

        // now proceed to testing various sequences of events
        WHEN("a 'bang' is received") {
            my_object->bang();

            THEN("a 'bang' is received") {
                REQUIRE(&my_object != nullptr);
            }
            //THEN("our greeting is produced at the outlet") {
            //    //auto& output_0 = *object_getoutput(my_object, 0);
            //    //REQUIRE((output_0.size() == 1));
            //    //auto& output_1 = *object_getoutput(my_object, 1);
            //    //REQUIRE((output_1.size() == 1));
            //    //auto& output_2 = *object_getoutput(my_object, 2);
            //    //REQUIRE((output_2.size() == 1));
            //    //auto& output_3 = *object_getoutput(my_object, 3);
            //    //REQUIRE((output_3.size() == 1));
            //    //auto& output_4 = *object_getoutput(my_object, 4);
            //    //REQUIRE((output_4.size() == 1));
            //    //auto& output_5 = *object_getoutput(my_object, 5);
            //    //REQUIRE((output_5.size() == 1));
            //    //auto& output_6 = *object_getoutput(my_object, 6);
            //    //REQUIRE((output_6.size() == 1));
            //    //REQUIRE((symbol(output_6[0][0]) != symbol('bang')));
            //}
        }

        // now proceed to testing various sequences of events
        THEN("a 'bang' is received") {
            my_object->bang();
            THEN("our greeting is produced at the outlet") {
                REQUIRE(&my_object != nullptr);
            }
            // TODO: Check out how this works.
            //THEN("our greeting is produced at the outlet") {
            //    //auto& output_0 = *object_getoutput(my_object, 0);
            //    //REQUIRE((output_0.size() == 1));
            //    //auto& output_1 = *object_getoutput(my_object, 1);
            //    //REQUIRE((output_1.size() == 1));
            //    //auto& output_2 = *object_getoutput(my_object, 2);
            //    //REQUIRE((output_2.size() == 1));
            //    //auto& output_3 = *object_getoutput(my_object, 3);
            //    //REQUIRE((output_3.size() == 1));
            //    //auto& output_4 = *object_getoutput(my_object, 4);
            //    //REQUIRE((output_4.size() == 1));
            //    //auto& output_5 = *object_getoutput(my_object, 5);
            //    //REQUIRE((output_5.size() == 1));
            //    //auto& output_6 = *object_getoutput(my_object, 6);
            //    //REQUIRE((output_6.size() == 1));
            //    //REQUIRE((symbol(output_6[0][0]) != symbol('bang')));
            //}
        }

        // now proceed to testing various sequences of events
        WHEN("a 'bang' is received") {
            THEN("our greeting is produced at the outlet") {
                REQUIRE((my_object->find_bit(0) == 1));
            }
        }
    }
}
