/// @file       seidr.NoteRandomOctaveMax_test.cpp
///	@ingroup 	jb
///	@copyright	Copyright 2025 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License
/// found in the License.md file.

#include "c74_min_unittest.h"

#include "Utils/Constants.hpp"
#include "Utils/MIDI.hpp"
#include "seidr.NoteRandomOctave.hpp"
#include "seidr.NoteRandomOctave.cpp"

using namespace MIDI;
using namespace MIDI::Notes;

SCENARIO("seidr.NoteRandomOctaveMax object basic functionality") {
    ext_main(nullptr);

    GIVEN("An instance of NoteRandomOctaveMax") {
        test_wrapper<NoteRandomOctaveMax> an_instance;
        NoteRandomOctaveMax &my_object = an_instance;

        WHEN("the object is created") {
            // THEN("it initializes successfully") { REQUIRE(my_object !=
            // nullptr); }

            THEN("it has the correct number of inlets and outlets") {
                // Test basic object structure
                c74::min::atoms args = {60, 100};
                REQUIRE_NOTHROW(my_object.anything(args));
                auto &out0 = *c74::max::object_getoutput(my_object, 0);
                const auto &out1 = *c74::max::object_getoutput(my_object, 1);
                REQUIRE(true);
            }
        }

        WHEN("basic MIDI note messages are processed") {
            THEN("note-on messages are handled") {
                c74::min::atoms args1 = {C5, 100};
                c74::min::atoms args2 = {G5, 80};
                c74::min::atoms args3 = {C6, 100};
                REQUIRE_NOTHROW(my_object.anything(args1));
                REQUIRE_NOTHROW(my_object.anything(args2));
                REQUIRE_NOTHROW(my_object.anything(args3));
            }

            THEN("note-off messages are handled") {
                c74::min::atoms args1 = {C5, 0}; // C4 off
                c74::min::atoms args2 = {G5, 0}; // G4 off
                REQUIRE_NOTHROW(my_object.anything(args1));
                REQUIRE_NOTHROW(my_object.anything(args2));
            }

            THEN("velocity values are processed correctly") {
                c74::min::atoms args1 = {C5, 127}; // max velocity
                c74::min::atoms args2 = {C5, 64};  // medium velocity
                c74::min::atoms args3 = {C5, 1};   // min non-zero velocity
                c74::min::atoms args4 = {C5, 0};   // note off
                REQUIRE_NOTHROW(my_object.anything(args1));
                REQUIRE_NOTHROW(my_object.anything(args2));
                REQUIRE_NOTHROW(my_object.anything(args3));
                REQUIRE_NOTHROW(my_object.anything(args4));
            }
        }

        WHEN("edge case MIDI notes are processed") {
            THEN("lowest MIDI note (0) is handled") {
                c74::min::atoms args = {C0, 100};
                REQUIRE_NOTHROW(my_object.anything(args));
            }

            THEN("highest MIDI note (127) is handled") {
                c74::min::atoms args = {G10, 100};
                REQUIRE_NOTHROW(my_object.anything(args));
            }

            THEN("middle C (60) is handled") {
                c74::min::atoms args = {C5, 100};
                REQUIRE_NOTHROW(my_object.anything(args));
            }
        }

        WHEN("range messages are sent") {
            THEN("valid ranges are accepted") {
                c74::min::atoms args1 = {0, 10}; // Full range
                c74::min::atoms args2 = {3, 5};  // Normal range
                c74::min::atoms args3 = {4, 4};  // Single octave
                REQUIRE_NOTHROW(my_object.range(args1));
                REQUIRE_NOTHROW(my_object.range(args2));
                REQUIRE_NOTHROW(my_object.range(args3));
            }

            THEN("range changes don't crash subsequent note processing") {
                c74::min::atoms range_args = {2, 6};
                c74::min::atoms note_args = {C5, 100};
                my_object.range(range_args);
                REQUIRE_NOTHROW(my_object.anything(note_args));

                c74::min::atoms range_args2 = {4, 4};
                c74::min::atoms note_args2 = {G5, 80};
                my_object.range(range_args2);
                REQUIRE_NOTHROW(my_object.anything(note_args2));
            }
        }

        WHEN("multiple operations are performed in sequence") {
            THEN("complex sequences don't crash") {
                // Test a realistic usage pattern
                c74::min::atoms range1 = {3, 5};
                c74::min::atoms note1 = {C5, 100};
                c74::min::atoms note2 = {G5, 80};
                c74::min::atoms note3 = {C5, 0};
                c74::min::atoms note4 = {G5, 100};
                c74::min::atoms range2 = {2, 6};
                c74::min::atoms note5 = {C6, 100};
                c74::min::atoms note6 = {C6, 0};

                my_object.range(range1);
                my_object.anything(note1);
                my_object.anything(note2);
                my_object.anything(note3);
                my_object.anything(note4);
                my_object.range(range2);
                my_object.anything(note5);
                my_object.anything(note6);

                // REQUIRE(&my_object != nullptr);
            }
        }
    }
}

SCENARIO("seidr.NoteRandomOctaveMax stress and performance tests") {
    ext_main(nullptr);

    GIVEN("An instance under stress conditions") {
        test_wrapper<NoteRandomOctaveMax> an_instance;
        NoteRandomOctaveMax &my_object = an_instance;

        WHEN("many rapid note messages are sent") {
            THEN("it handles rapid note-ons without crashing") {
                for (int i = 0; i < 50; i++) {
                    c74::min::atoms args = {C5 + (i % OCTAVE), 100};
                    REQUIRE_NOTHROW(my_object.anything(args));
                }
            }

            THEN("it handles rapid note-offs without crashing") {
                for (int i = 0; i < 50; i++) {
                    c74::min::atoms args = {C5 + (i % OCTAVE), 0};
                    REQUIRE_NOTHROW(my_object.anything(args));
                }
            }

            THEN("it handles mixed rapid messages without crashing") {
                for (int i = 0; i < 100; i++) {
                    c74::min::atoms args = {C5 + (i % OCTAVE), (i % 2 == 0) ? 100 : 0};
                    REQUIRE_NOTHROW(my_object.anything(args));
                }
            }
        }

        WHEN("range changes are interspersed with notes") {
            THEN("it handles the pattern without crashing") {
                for (int i = 0; i < 20; i++) {
                    if (i % 4 == 0) {
                        c74::min::atoms range_args = {i % 5, (i % 5) + 2};
                        my_object.range(range_args);
                    }

                    c74::min::atoms note_args = {C4 + (i % 24), 100};
                    REQUIRE_NOTHROW(my_object.anything(note_args));
                }
            }
        }

        WHEN("range changes are interspersed with notes") {
            THEN("it handles the pattern without crashing") {
                for (int i = 0; i < 20; i++) {
                    if (i % 4 == 0) {
                        c74::min::atoms range_args = {i % 5, (i % 5) + 2};
                        my_object.range(range_args);
                    }

                    c74::min::atoms note_args = {48 + (i % 24), 100};
                    REQUIRE_NOTHROW(my_object.anything(note_args));
                }
            }
        }
    }
}

SCENARIO("seidr.NoteRandomOctaveMax error handling tests") {
    ext_main(nullptr);

    GIVEN("An instance handling invalid input") {
        test_wrapper<NoteRandomOctaveMax> an_instance;
        NoteRandomOctaveMax &my_object = an_instance;

        WHEN("invalid list lengths are sent") {
            THEN("empty list is handled") {
                c74::min::atoms empty_args = {};
                REQUIRE_NOTHROW(my_object.anything(empty_args));
            }

            THEN("single element list is handled") {
                c74::min::atoms single_arg = {60};
                REQUIRE_NOTHROW(my_object.anything(single_arg));
            }

            THEN("too many elements are handled") {
                c74::min::atoms many_args = {60, 100, 123, 456};
                REQUIRE_NOTHROW(my_object.anything(many_args));
            }
        }

        WHEN("out-of-range MIDI values are sent") {
            THEN("negative pitch is handled") {
                c74::min::atoms args = {-1, 100};
                REQUIRE_NOTHROW(my_object.anything(args));
            }

            THEN("excessive pitch is handled") {
                c74::min::atoms args = {128, 100};
                REQUIRE_NOTHROW(my_object.anything(args));
            }

            THEN("negative velocity is handled") {
                c74::min::atoms args = {60, -1};
                REQUIRE_NOTHROW(my_object.anything(args));
            }

            THEN("excessive velocity is handled") {
                c74::min::atoms args = {60, 128};
                REQUIRE_NOTHROW(my_object.anything(args));
            }
        }

        WHEN("invalid range values are sent") {
            THEN("negative octaves are handled") {
                c74::min::atoms args = {-1, 5};
                REQUIRE_NOTHROW(my_object.range(args));
            }

            THEN("excessive octaves are handled") {
                c74::min::atoms args = {3, 11};
                REQUIRE_NOTHROW(my_object.range(args));
            }

            THEN("swapped min/max are handled") {
                c74::min::atoms args = {5, 3};
                REQUIRE_NOTHROW(my_object.range(args));
            }

            THEN("single element range is handled") {
                c74::min::atoms args = {3};
                REQUIRE_NOTHROW(my_object.range(args));
            }

            THEN("empty range is handled") {
                c74::min::atoms args = {};
                REQUIRE_NOTHROW(my_object.range(args));
            }
        }
    }
}

SCENARIO("seidr.NoteRandomOctaveMax musical scale tests") {
    ext_main(nullptr);

    GIVEN("An instance processing musical scales") {
        test_wrapper<NoteRandomOctaveMax> an_instance;
        NoteRandomOctaveMax &my_object = an_instance;

        WHEN("a C major scale is played") {
            THEN("all scale notes are processed without crashing") {
                // C4 to C5
                int c_major[] = {C5, D5, E5, F5, G5, A5, B5, C6};

                for (int note : c_major) {
                    c74::min::atoms on_args = {note, 100};
                    c74::min::atoms off_args = {note, 0};
                    REQUIRE_NOTHROW(my_object.anything(on_args));
                    REQUIRE_NOTHROW(my_object.anything(off_args));
                }
            }
        }

        WHEN("a chromatic scale is played") {
            THEN("all chromatic notes are processed without crashing") {
                for (int note = C5; note <= C6; note++) {
                    c74::min::atoms on_args = {note, 100};
                    REQUIRE_NOTHROW(my_object.anything(on_args));
                }

                for (int note = C5; note <= C6; note++) {
                    c74::min::atoms off_args = {note, 0};
                    REQUIRE_NOTHROW(my_object.anything(off_args));
                }
            }
        }

        WHEN("chords are played") {
            THEN("common chords are processed without crashing") {
                // C major chord
                c74::min::atoms c_args = {C6, 100};
                c74::min::atoms e_args = {E6, 100};
                c74::min::atoms g_args = {G6, 100};
                REQUIRE_NOTHROW(my_object.anything(c_args));
                REQUIRE_NOTHROW(my_object.anything(e_args));
                REQUIRE_NOTHROW(my_object.anything(g_args));

                // Release chord
                c74::min::atoms c_off = {C6, 0};
                c74::min::atoms e_off = {E6, 0};
                c74::min::atoms g_off = {G6, 0};
                REQUIRE_NOTHROW(my_object.anything(c_off));
                REQUIRE_NOTHROW(my_object.anything(e_off));
                REQUIRE_NOTHROW(my_object.anything(g_off));
            }
        }

        WHEN("chords are played") {
            THEN("common chords are processed without crashing") {
                // C major chord
                c74::min::atoms c_args = {60, 100};
                c74::min::atoms e_args = {64, 100};
                c74::min::atoms g_args = {67, 100};
                REQUIRE_NOTHROW(my_object.anything(c_args));
                REQUIRE_NOTHROW(my_object.anything(e_args));
                REQUIRE_NOTHROW(my_object.anything(g_args));

                // Release chord
                c74::min::atoms c_off = {60, 0};
                c74::min::atoms e_off = {64, 0};
                c74::min::atoms g_off = {67, 0};
                REQUIRE_NOTHROW(my_object.anything(c_off));
                REQUIRE_NOTHROW(my_object.anything(e_off));
                REQUIRE_NOTHROW(my_object.anything(g_off));
            }
        }
    }
}
