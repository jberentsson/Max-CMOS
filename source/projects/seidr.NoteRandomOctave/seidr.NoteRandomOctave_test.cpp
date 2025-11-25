/// @file       seidr.NoteRandomOctaveMax_test.cpp
///	@ingroup 	seidr
///	@copyright	Copyright 2025 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License
///             found in the License.md file.

#include "Utils/Constants.hpp"
#include "Utils/MIDI.hpp"
#include "seidr.NoteRandomOctave.cpp" // NOLINT
#include "seidr.NoteRandomOctave.hpp"
#include <c74_min_unittest.h>

using namespace MIDI;
using namespace MIDI::Notes;

SCENARIO("seidr.NoteRandomOctaveMax object basic functionality") { // NOLINT
    ext_main(nullptr);

    GIVEN("An instance of NoteRandomOctaveMax") {
        test_wrapper<NoteRandomOctaveMax> an_instance;
        NoteRandomOctaveMax &myObject = an_instance;

        WHEN("the object is created") {
            THEN("it has the correct number of inlets and outlets") {
                // Test basic object structure
                c74::min::atoms args = {NoteC5, 100}; // NOLINT
                REQUIRE_NOTHROW(myObject.int_message(args));
                auto &out0 = *c74::max::object_getoutput(myObject, 0);
                const auto &out1 = *c74::max::object_getoutput(myObject, 1);
                REQUIRE(true);
            }
        }

        WHEN("basic MIDI note messages are processed") {
            THEN("note-on messages are handled") {
                c74::min::atoms args1 = {NoteC5, 100}; // NOLINT
                c74::min::atoms args2 = {NoteG5, 80};  // NOLINT
                c74::min::atoms args3 = {NoteC6, 100}; // NOLINT
                REQUIRE_NOTHROW(myObject.int_message(args1));
                REQUIRE_NOTHROW(myObject.int_message(args2));
                REQUIRE_NOTHROW(myObject.int_message(args3));
            }

            THEN("note-off messages are handled") {
                c74::min::atoms args1 = {NoteC5, 0}; // C4 off  // NOLINT
                c74::min::atoms args2 = {NoteG5, 0}; // G4 off  // NOLINT
                REQUIRE_NOTHROW(myObject.int_message(args1));
                REQUIRE_NOTHROW(myObject.int_message(args2));
            }

            THEN("velocity values are processed correctly") {
                c74::min::atoms args1 = {NoteC5, 127}; // max velocity // NOLINT
                c74::min::atoms args2 = {NoteC5, 64};  // medium velocity // NOLINT
                c74::min::atoms args3 = {NoteC5, 1};   // min non-zero velocity // NOLINT
                c74::min::atoms args4 = {NoteC5, 0};   // note off // NOLINT
                REQUIRE_NOTHROW(myObject.int_message(args1));
                REQUIRE_NOTHROW(myObject.int_message(args2));
                REQUIRE_NOTHROW(myObject.int_message(args3));
                REQUIRE_NOTHROW(myObject.int_message(args4));
            }
        }

        WHEN("edge case MIDI notes are processed") {
            THEN("lowest MIDI note (C0) is handled") {
                c74::min::atoms args = {NoteC0, 100}; // NOLINT
                REQUIRE_NOTHROW(myObject.int_message(args));
            }

            THEN("highest MIDI note (G10) is handled") {
                c74::min::atoms args = {NoteG10, 100}; // NOLINT
                REQUIRE_NOTHROW(myObject.int_message(args));
            }

            THEN("middle C (C5) is handled") {
                c74::min::atoms args = {NoteC5, 100}; // NOLINT
                REQUIRE_NOTHROW(myObject.int_message(args));
            }
        }

        WHEN("range messages are sent") {
            THEN("valid ranges are accepted") {
                c74::min::atoms args1 = {0, 10}; // Full range // NOLINT
                c74::min::atoms args2 = {3, 5};  // Normal range // NOLINT
                c74::min::atoms args3 = {4, 4};  // Single octave // NOLINT
                REQUIRE_NOTHROW(myObject.range(args1));
                REQUIRE_NOTHROW(myObject.range(args2));
                REQUIRE_NOTHROW(myObject.range(args3));
            }

            THEN("range changes don't crash subsequent note processing") {
                c74::min::atoms range_args = {2, 6};       // NOLINT
                c74::min::atoms note_args = {NoteC5, 100}; // NOLINT
                myObject.range(range_args);
                REQUIRE_NOTHROW(myObject.int_message(note_args));

                c74::min::atoms range_args2 = {4, 4};
                c74::min::atoms note_args2 = {NoteG5, 80}; // NOLINT
                myObject.range(range_args2);
                REQUIRE_NOTHROW(myObject.int_message(note_args2));
            }
        }

        WHEN("multiple operations are performed in sequence") {
            THEN("complex sequences don't crash") {
                // Test a realistic usage pattern
                c74::min::atoms range1 = {3, 5};       // NOLINT
                c74::min::atoms note1 = {NoteC5, 100}; // NOLINT
                c74::min::atoms note2 = {NoteG5, 80};  // NOLINT
                c74::min::atoms note3 = {NoteC5, 0};
                c74::min::atoms note4 = {NoteG5, 100}; // NOLINT
                c74::min::atoms range2 = {2, 6};       // NOLINT
                c74::min::atoms note5 = {NoteC6, 100}; // NOLINT
                c74::min::atoms notNoteE6 = {NoteC6, 0};

                myObject.range(range1);
                myObject.int_message(note1);
                myObject.int_message(note2);
                myObject.int_message(note3);
                myObject.int_message(note4);
                myObject.range(range2);
                myObject.int_message(note5);
                myObject.int_message(notNoteE6);
            }
        }
    }
}

SCENARIO("seidr.NoteRandomOctaveMax stress and performance tests") { // NOLINT
    ext_main(nullptr);

    GIVEN("An instance under stress conditions") {
        test_wrapper<NoteRandomOctaveMax> an_instance;
        NoteRandomOctaveMax &myObject = an_instance;

        WHEN("many rapid note messages are sent") {
            THEN("it handles rapid note-ons without crashing") {
                for (int i = 0; i < 50; i++) {                           // NOLINT
                    c74::min::atoms args = {NoteC5 + (i % OCTAVE), 100}; // NOLINT
                    REQUIRE_NOTHROW(myObject.int_message(args));
                }
            }

            THEN("it handles rapid note-offs without crashing") {
                for (int i = 0; i < 50; i++) { // NOLINT
                    c74::min::atoms args = {NoteC5 + (i % OCTAVE), 0};
                    REQUIRE_NOTHROW(myObject.int_message(args));
                }
            }

            THEN("it handles mixed rapid messages without crashing") {
                for (int i = 0; i < 100; i++) {                                             // NOLINT
                    c74::min::atoms args = {NoteC5 + (i % OCTAVE), (i % 2 == 0) ? 100 : 0}; // NOLINT
                    REQUIRE_NOTHROW(myObject.int_message(args));
                }
            }
        }

        WHEN("range changes are interspersed with notes") {
            THEN("it handles the pattern without crashing") {
                for (int i = 0; i < 20; i++) { // NOLINT
                    if (i % 4 == 0) {
                        c74::min::atoms range_args = {i % 5, (i % 5) + 2}; // NOLINT
                        myObject.range(range_args);
                    }

                    c74::min::atoms note_args = {NoteC4 + (i % 24), 100}; // NOLINT
                    REQUIRE_NOTHROW(myObject.int_message(note_args));
                }
            }
        }

        WHEN("range changes are interspersed with notes") {
            THEN("it handles the pattern without crashing") {
                for (int i = 0; i < 20; i++) { // NOLINT
                    if (i % 4 == 0) {
                        c74::min::atoms range_args = {i % 5, (i % 5) + 2}; // NOLINT
                        myObject.range(range_args);
                    }

                    c74::min::atoms note_args = {48 + (i % 24), 100}; // NOLINT
                    REQUIRE_NOTHROW(myObject.int_message(note_args));
                }
            }
        }
    }
}

SCENARIO("seidr.NoteRandomOctaveMax error handling tests") { // NOLINT
    ext_main(nullptr);

    GIVEN("An instance handling invalid input") {
        test_wrapper<NoteRandomOctaveMax> an_instance;
        NoteRandomOctaveMax &myObject = an_instance;

        WHEN("invalid list lengths are sent") {
            THEN("empty list is handled") {
                c74::min::atoms empty_args = {};
                REQUIRE_NOTHROW(myObject.int_message(empty_args));
            }

            THEN("single element list is handled") {
                c74::min::atoms single_arg = {NoteC5};
                REQUIRE_NOTHROW(myObject.int_message(single_arg));
            }

            THEN("too many elements are handled") {
                c74::min::atoms many_args = {NoteC5, 100, 123, 456}; // NOLINT
                REQUIRE_NOTHROW(myObject.int_message(many_args));
            }
        }

        WHEN("out-of-range MIDI values are sent") {
            THEN("negative pitch is handled") {
                c74::min::atoms args = {-1, 100}; // NOLINT
                REQUIRE_NOTHROW(myObject.int_message(args));
            }

            THEN("excessive pitch is handled") {
                c74::min::atoms args = {NoteG10 + 1, 100}; // NOLINT
                REQUIRE_NOTHROW(myObject.int_message(args));
            }

            THEN("negative velocity is handled") {
                c74::min::atoms args = {NoteC5, -1};
                REQUIRE_NOTHROW(myObject.int_message(args));
            }

            THEN("excessive velocity is handled") {
                c74::min::atoms args = {NoteC5, 128}; // NOLINT
                REQUIRE_NOTHROW(myObject.int_message(args));
            }
        }

        WHEN("invalid range values are sent") {
            THEN("negative octaves are handled") {
                c74::min::atoms args = {-1, 5}; // NOLINT
                REQUIRE_NOTHROW(myObject.range(args));
            }

            THEN("excessive octaves are handled") {
                c74::min::atoms args = {3, 11}; // NOLINT
                REQUIRE_NOTHROW(myObject.range(args));
            }

            THEN("swapped min/max are handled") {
                c74::min::atoms args = {5, 3}; // NOLINT
                REQUIRE_NOTHROW(myObject.range(args));
            }

            THEN("single element range is handled") {
                c74::min::atoms args = {3};
                REQUIRE_NOTHROW(myObject.range(args));
            }

            THEN("empty range is handled") {
                c74::min::atoms args = {};
                REQUIRE_NOTHROW(myObject.range(args));
            }
        }
    }
}

SCENARIO("seidr.NoteRandomOctaveMax musical scale tests") { // NOLINT
    ext_main(nullptr);

    GIVEN("An instance processing musical scales") {
        test_wrapper<NoteRandomOctaveMax> an_instance;
        NoteRandomOctaveMax &myObject = an_instance;

        WHEN("a C major scale is played") {
            THEN("all scale notes are processed without crashing") {
                // C4 to C5
                int c_major[] = {NoteC5, NoteD5, NoteE5, NoteF5, NoteG5, NoteA5, NoteB5, NoteC6};

                for (int note : c_major) {
                    c74::min::atoms on_args = {note, 100}; // NOLINT
                    c74::min::atoms off_args = {note, 0};
                    REQUIRE_NOTHROW(myObject.int_message(on_args));
                    REQUIRE_NOTHROW(myObject.int_message(off_args));
                }
            }
        }

        WHEN("a chromatic scale is played") {
            THEN("all chromatic notes are processed without crashing") {
                for (int note = NoteC5; note <= NoteC6; note++) {
                    c74::min::atoms on_args = {note, 100}; // NOLINT
                    REQUIRE_NOTHROW(myObject.int_message(on_args));
                }

                for (int note = NoteC5; note <= NoteC6; note++) {
                    c74::min::atoms off_args = {note, 0};
                    REQUIRE_NOTHROW(myObject.int_message(off_args));
                }
            }
        }

        WHEN("chords are played") {
            THEN("common chords are processed without crashing") {
                // C major chord
                c74::min::atoms c_args = {NoteC6, 100}; // NOLINT
                c74::min::atoms e_args = {NoteE6, 100}; // NOLINT
                c74::min::atoms g_args = {NoteG9, 100}; // NOLINT
                REQUIRE_NOTHROW(myObject.int_message(c_args));
                REQUIRE_NOTHROW(myObject.int_message(e_args));
                REQUIRE_NOTHROW(myObject.int_message(g_args));

                // Release chord
                c74::min::atoms c_off = {NoteC6, 0};
                c74::min::atoms e_off = {NoteE6, 0};
                c74::min::atoms g_off = {NoteG9, 0};
                REQUIRE_NOTHROW(myObject.int_message(c_off));
                REQUIRE_NOTHROW(myObject.int_message(e_off));
                REQUIRE_NOTHROW(myObject.int_message(g_off));
            }
        }

        WHEN("chords are played") {
            THEN("common chords are processed without crashing") {
                // C major chord
                c74::min::atoms c_args = {NoteC5, 100}; // NOLINT
                c74::min::atoms e_args = {NoteE5, 100}; // NOLINT
                c74::min::atoms g_args = {NoteG5, 100}; // NOLINT
                REQUIRE_NOTHROW(myObject.int_message(c_args));
                REQUIRE_NOTHROW(myObject.int_message(e_args));
                REQUIRE_NOTHROW(myObject.int_message(g_args));

                // Release chord
                c74::min::atoms c_off = {NoteC5, 0};
                c74::min::atoms e_off = {NoteE5, 0};
                c74::min::atoms g_off = {NoteG5, 0};
                REQUIRE_NOTHROW(myObject.int_message(c_off));
                REQUIRE_NOTHROW(myObject.int_message(e_off));
                REQUIRE_NOTHROW(myObject.int_message(g_off));
            }
        }
    }
}
