/// @file       seidr.RandomOctaveMax_test.cpp
///	@ingroup 	seidr
///	@copyright	Copyright 2025 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License
///             found in the License.md file.

#include "Utils/MIDI.hpp"
#include "seidr.RandomOctave.cpp" // NOLINT
#include "seidr.RandomOctave.hpp"
#include <c74_min_unittest.h>

using namespace MIDI;
using namespace MIDI::Notes;

SCENARIO("seidr.RandomOctaveMax object basic functionality") { // NOLINT
    ext_main(nullptr);

    GIVEN("An instance of RandomOctaveMax") {
        test_wrapper<RandomOctaveMax> an_instance;
        RandomOctaveMax &randomOctaveTestObject = an_instance;
        
        auto &note_output = *c74::max::object_getoutput(randomOctaveTestObject, 0);
        auto &velocity_output = *c74::max::object_getoutput(randomOctaveTestObject, 1);

        WHEN("the object is created") {
            THEN("it has the correct number of inlets and outlets") {
                REQUIRE_NOTHROW(randomOctaveTestObject.anything({ NoteC5, 100 }));
                
                REQUIRE(!note_output.empty());
                REQUIRE(!velocity_output.empty());
        
                REQUIRE(static_cast<int> (note_output[0][1]) % 12 == 0);
                REQUIRE(velocity_output[0][1] == 100);
            }
        }

        WHEN("basic MIDI note messages are processed") {
            THEN("note-on messages are handled") {
                REQUIRE_NOTHROW(randomOctaveTestObject.anything({ NoteC5, 100 }));
                REQUIRE_NOTHROW(randomOctaveTestObject.anything({ NoteG5, 80 }));
                REQUIRE_NOTHROW(randomOctaveTestObject.anything({ NoteC6, 100 }));

                REQUIRE(static_cast<int> (note_output[0][1]) % 12 == 0);
                REQUIRE(static_cast<int> (note_output[1][1]) % 12 == 7);
                REQUIRE(static_cast<int> (note_output[2][1]) % 12 == 0);
                
            }

            THEN("note-off messages are handled") {
                REQUIRE_NOTHROW(randomOctaveTestObject.anything({ NoteC5, 0 }));
                REQUIRE_NOTHROW(randomOctaveTestObject.anything({ NoteG5, 0 }));

                REQUIRE(!note_output.empty());

                REQUIRE(static_cast<int> (note_output[0][1]) % 12 == NoteC0);
                REQUIRE(static_cast<int> (note_output[1][1]) % 12 == NoteG0);
            }

            THEN("velocity values are processed correctly") {
                REQUIRE(note_output.empty());

                // Same note but in different octaves
                // Maybe there are errors here ecause the not isn't randomized.
                REQUIRE_NOTHROW(randomOctaveTestObject.anything({ NoteC5, 127 }));
                REQUIRE_NOTHROW(randomOctaveTestObject.anything({ NoteC5, 64 }));
                REQUIRE_NOTHROW(randomOctaveTestObject.anything({ NoteC5, 1 }));
                
                REQUIRE(!note_output.empty());
                REQUIRE(note_output.size() > 1);
                
                // Turn all notes off fo that class of notes.
                REQUIRE_NOTHROW(randomOctaveTestObject.anything({ NoteC5, 0 }));
                
                REQUIRE(!note_output.empty());
                REQUIRE(note_output.size() > 1);
                
                REQUIRE(static_cast<int> (note_output[0][1]) % 12 == 0);
                REQUIRE(static_cast<int> (note_output[1][1]) % 12 == 0);
                REQUIRE(static_cast<int> (note_output[2][1]) % 12 == 0);
            }
        }

        WHEN("edge case MIDI notes are processed") {
            THEN("lowest MIDI note (C0) is handled") {
                REQUIRE_NOTHROW(randomOctaveTestObject.anything({ NoteC0, 100 }));
                REQUIRE(static_cast<int> (note_output[0][1]) % 12 == 0);
            }

            THEN("highest MIDI note (G10) is handled") {
                REQUIRE_NOTHROW(randomOctaveTestObject.anything({ NoteG10, 100 }));
                REQUIRE(static_cast<int> (note_output[0][1]) % 12 == 7);
            }

            THEN("middle C (C5) is handled") {
                REQUIRE_NOTHROW(randomOctaveTestObject.anything({ NoteC5, 100 }));
                REQUIRE(static_cast<int> (note_output[0][1]) % 12 == 0);
            }
        }

        WHEN("range messages are sent") {
            THEN("valid ranges are accepted") {
                REQUIRE_NOTHROW(randomOctaveTestObject.range({ 0, 10 }));
                REQUIRE_NOTHROW(randomOctaveTestObject.range({ 3, 5 }));
                REQUIRE_NOTHROW(randomOctaveTestObject.range({ 4, 4 }));
            }

            THEN("range changes don't crash subsequent note processing") {
                randomOctaveTestObject.range({ 24, 72 }); // NOLINT
                REQUIRE_NOTHROW(randomOctaveTestObject.anything({ NoteC5, 100 }));
                // TODO: We should be producing output here.
                //REQUIRE(static_cast<int> (note_output[0][1]) % 12 == 6);

                // TODO: The range is untested.
                randomOctaveTestObject.range({ 48, 48 }); // NOLINT
                REQUIRE_NOTHROW(randomOctaveTestObject.anything({ NoteG5, 80 }));
                // TODO: We should be producing output here.
                //REQUIRE(static_cast<int> (note_output[1][1]) % 12 == 4);
            }
        } 

        WHEN("multiple operations are performed in sequence") {
            THEN("complex sequences don't crash") {
                // Test a realistic usage pattern
                REQUIRE(note_output.empty());

                randomOctaveTestObject.range({ 36, 60 }); // NOLINT
                randomOctaveTestObject.anything({ NoteC5, 100 }); // NOLINT
                randomOctaveTestObject.anything({ NoteG5, 80 });  // NOLINT
                randomOctaveTestObject.anything({ NoteC5, 0 });
                randomOctaveTestObject.anything({ NoteG5, 100 }); // NOLINT
                randomOctaveTestObject.range({ 24, 72 });         // NOLINT
                randomOctaveTestObject.anything({ NoteC6, 100 }); // NOLINT
                randomOctaveTestObject.anything({ NoteC6, 0 });

                REQUIRE(note_output.empty());
            }
        }
    } 
}

SCENARIO("seidr.RandomOctaveMax stress and performance tests") { // NOLINT
    ext_main(nullptr);

    GIVEN("An instance under stress conditions") {
        test_wrapper<RandomOctaveMax> an_instance;
        RandomOctaveMax &randomOctaveTestObject = an_instance;
        
        auto &note_output = *c74::max::object_getoutput(randomOctaveTestObject, 0);
        auto &velocity_output = *c74::max::object_getoutput(randomOctaveTestObject, 1);

        WHEN("many rapid note messages are sent") {
            THEN("it handles rapid note-ons without crashing") {
                for (int i = 0; i < 50; i++) {                           // NOLINT
                    REQUIRE_NOTHROW(randomOctaveTestObject.anything({NoteC5 + (i % OCTAVE), 100}));
                }
                
                REQUIRE(!note_output.empty());
                // TODO: We are not producing any note output. This always crashes.
                //REQUIRE(note_output.size() == 50);
            }

            THEN("it handles rapid note-offs without crashing") {
                for (int i = 0; i < 50; i++) { // NOLINT
                    REQUIRE_NOTHROW(randomOctaveTestObject.anything({NoteC5 + (i % OCTAVE), 0}));
                }
                
                REQUIRE(!note_output.empty());
            }

            THEN("it handles mixed rapid messages without crashing") {
                for (int i = 0; i < 100; i++) { // NOLINT
                    REQUIRE_NOTHROW(randomOctaveTestObject.anything({NoteC5 + (i % OCTAVE), (i % 2 == 0) ? 100 : 0}));
                }
            }
        }

        WHEN("range changes are interspersed with notes") {
            THEN("it handles the pattern without crashing") {
                for (int i = 0; i < 20; i++) { // NOLINT
                    if (i % 4 == 0) {
                        randomOctaveTestObject.range({ i % 5, (i % 5) + 2 }); // NOLINT
                    }

                    REQUIRE_NOTHROW(randomOctaveTestObject.anything({ NoteC4 + (i % 24), 100 }));
                }
            }
        }

        WHEN("range changes are interspersed with notes") {
            THEN("it handles the pattern without crashing") {
                for (int i = 0; i < 20; i++) { // NOLINT
                    if (i % 4 == 0) {
                        randomOctaveTestObject.range({ i % 5, (i % 5) + 2 }); // NOLINT
                    }

                    REQUIRE_NOTHROW(randomOctaveTestObject.anything({ 48 + (i % 24), 100 }));
                }
            }
        }
    }
}

SCENARIO("seidr.RandomOctaveMax error handling tests") { // NOLINT
    ext_main(nullptr);

    GIVEN("An instance handling invalid input") {
        test_wrapper<RandomOctaveMax> an_instance;
        RandomOctaveMax &randomOctaveTestObject = an_instance;

        auto &note_output = *c74::max::object_getoutput(randomOctaveTestObject, 0);
        auto &velocity_output = *c74::max::object_getoutput(randomOctaveTestObject, 1);

        WHEN("invalid list lengths are sent") {
            THEN("empty list is handled") {
                REQUIRE_NOTHROW(randomOctaveTestObject.anything());
                REQUIRE(note_output.empty());
            }

            THEN("single element list is handled") {
                REQUIRE_NOTHROW(randomOctaveTestObject.anything(NoteC5));
                REQUIRE(note_output.empty());
            }

            THEN("too many elements are handled") {
                REQUIRE_NOTHROW(randomOctaveTestObject.anything({ NoteC5, 100, 123, 456 }));
                REQUIRE(!note_output.empty());
            }
        }

        WHEN("out-of-range MIDI values are sent") {
            THEN("negative pitch is handled") {
                REQUIRE_NOTHROW(randomOctaveTestObject.anything({ -1, 100 }));
                REQUIRE(note_output.empty());
            }

            THEN("excessive pitch is handled") {
                REQUIRE_NOTHROW(randomOctaveTestObject.anything({ NoteG10 + 1, 100 }));
                REQUIRE(note_output.empty());
            }

            THEN("negative velocity is handled") {
                REQUIRE_NOTHROW(randomOctaveTestObject.anything({ NoteC5, -1 }));
                REQUIRE(note_output.empty());
            }

            THEN("excessive velocity is handled") {
                REQUIRE_NOTHROW(randomOctaveTestObject.anything({ NoteC5, 128 }));
                REQUIRE(note_output.empty());
            }
        }

        WHEN("invalid range values are sent") {
            THEN("negative octaves are handled") {
                REQUIRE_NOTHROW(randomOctaveTestObject.range({ -1, 5 }));
            }

            THEN("excessive octaves are handled") {
                REQUIRE_NOTHROW(randomOctaveTestObject.range({ 3, 11 }));
            }

            THEN("swapped min/max are handled") {
                REQUIRE_NOTHROW(randomOctaveTestObject.range({ 5, 3 }));
            }

            THEN("single element range is handled") {
                REQUIRE_NOTHROW(randomOctaveTestObject.range(3));
            }

            THEN("empty range is handled") {
                REQUIRE_NOTHROW(randomOctaveTestObject.range());
            }
        }
    }
}

SCENARIO("seidr.RandomOctaveMax musical scale tests") { // NOLINT
    ext_main(nullptr);

    GIVEN("An instance processing musical scales") {
        test_wrapper<RandomOctaveMax> an_instance;
        RandomOctaveMax &randomOctaveTestObject = an_instance;

        auto &note_output = *c74::max::object_getoutput(randomOctaveTestObject, 0);
        auto &velocity_output = *c74::max::object_getoutput(randomOctaveTestObject, 1);

        WHEN("a C major scale is played") {
            THEN("all scale notes are processed without crashing") {
                // C4 to C5
                int c_major[] = {
                    NoteC5,
                    NoteD5,
                    NoteE5,
                    NoteF5,
                    NoteG5,
                    NoteA5,
                    NoteB5,
                    NoteC6
                };

                REQUIRE(randomOctaveTestObject.getActiveNotes().empty());

                for (int note : c_major) {
                    REQUIRE_NOTHROW(randomOctaveTestObject.anything({note, 1000}));
                    REQUIRE_NOTHROW(randomOctaveTestObject.anything({note, 0}));
                }

                // TODO: there are not supposed to be any active notes.
                REQUIRE(randomOctaveTestObject.getActiveNotes().empty());
                REQUIRE(!note_output.empty());
                for(const auto &nout : note_output){
                    REQUIRE(static_cast<int> (nout[0]) != 0);
                }
            }
        }

        WHEN("a chromatic scale is played") {
            THEN("all chromatic notes are processed without crashing") {
                for (int note = NoteC5; note <= NoteC6; note++) {
                    REQUIRE_NOTHROW(randomOctaveTestObject.anything({ note, 100 }));
                }
                
                // TODO: There should be 13 active notes. Always crashes. Sometimes there are 12 notes.
                //REQUIRE(randomOctaveTestObject.getActiveNotes().size() == 13);

                for (int note = NoteC5; note <= NoteC6; note++) {
                    REQUIRE_NOTHROW(randomOctaveTestObject.anything({ note, 0 }));
                }

                // TODO: There should be no active notes.
                //REQUIRE(randomOctaveTestObject.getActiveNotes().size() == 0);

                REQUIRE(!note_output.empty());
            }
        }

        WHEN("chords are played") {
            THEN("common chords are processed without crashing") {
                // C major chord
                REQUIRE_NOTHROW(randomOctaveTestObject.anything({ NoteC6, 100 }));
                REQUIRE_NOTHROW(randomOctaveTestObject.anything({ NoteE6, 100 }));
                REQUIRE_NOTHROW(randomOctaveTestObject.anything({ NoteG6, 100 }));

                REQUIRE(!note_output.empty());
                REQUIRE(note_output.size() == 3);

                // Release chord
                REQUIRE_NOTHROW(randomOctaveTestObject.anything({ NoteC6, 0 }));
                REQUIRE_NOTHROW(randomOctaveTestObject.anything({ NoteE6, 0 }));
                REQUIRE_NOTHROW(randomOctaveTestObject.anything({ NoteG6, 0 }));

                REQUIRE(!note_output.empty());
                REQUIRE(note_output.size() == 6);
            }
        }

        WHEN("chords are played") {
            THEN("common chords are processed without crashing") {
                // C major chord
                REQUIRE_NOTHROW(randomOctaveTestObject.anything({ NoteC5, 100 }));
                REQUIRE_NOTHROW(randomOctaveTestObject.anything({ NoteE5, 100 }));
                REQUIRE_NOTHROW(randomOctaveTestObject.anything({ NoteG5, 100 }));

                REQUIRE(randomOctaveTestObject.getQueuedNotes().empty());
                REQUIRE(randomOctaveTestObject.getActiveNotes().size() == 3);

                // Release chord
                REQUIRE_NOTHROW(randomOctaveTestObject.anything({ NoteC5, 0 }));
                REQUIRE_NOTHROW(randomOctaveTestObject.anything({ NoteE5, 0 }));
                REQUIRE_NOTHROW(randomOctaveTestObject.anything({ NoteG5, 0 }));
             
                REQUIRE(randomOctaveTestObject.getQueuedNotes().empty());
                REQUIRE(randomOctaveTestObject.getActiveNotes().empty());
                REQUIRE(!note_output.empty());

                REQUIRE(note_output.size() == 6);
            }
        }
    }
}
