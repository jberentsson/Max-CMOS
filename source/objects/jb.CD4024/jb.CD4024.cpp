/// @file       jb.CD4024.cpp
///	@ingroup 	jb
///	@copyright	Copyright 2018 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License found in the License.md file.

#include "c74_min.h"
#include <string>
#include <iostream>
#include <io.h>
#include <fcntl.h>

using namespace c74::min;


class CD4024 : public object<CD4024> {
public:
    MIN_DESCRIPTION	{"CMOS CD4024"};
    MIN_TAGS		{"cmos"};
    MIN_AUTHOR		{"Jóhann Berentsson"};
    MIN_RELATED		{"print, jit.print, dict.print"};

    inlet<>  input_0 { this, "(bang) input pulse" };
    inlet<>  input_1 { this, "(reset) reset pulse" };
    outlet<> output_0{ this, "(anything) output bit 0" };
    outlet<> output_1{ this, "(anything) output bit 1" };
    outlet<> output_2{ this, "(anything) output bit 2" };
    outlet<> output_3{ this, "(anything) output bit 3" };
    outlet<> output_4{ this, "(anything) output bit 4" };
    outlet<> output_5{ this, "(anything) output bit 5" };
    outlet<> output_6{ this, "(anything) output bit 6" };

    int counter = -1;
    int* counter_ptr = &counter;
    bool bang_enabled = FALSE;
    //outlet<>* outputs[7] = { &output_0, &output_1, &output_2, &output_3, &output_4, &output_5, &output_6 };

    int find_bit(int output) {
        // isolate the correct bit.
        return ((counter) >> output) & 0x1;
    }

    void handle_output() {
        // Send data to the outputs
        // TODO: Send only data on change

        if (bang_enabled) {
            send_bangs();
        } else {
            output_0.send(this->find_bit(0));
            output_1.send(this->find_bit(1));
            output_2.send(this->find_bit(2));
            output_3.send(this->find_bit(3));
            output_4.send(this->find_bit(4));
            output_5.send(this->find_bit(5));
            output_6.send(this->find_bit(6));
        }
    }

    void send_bangs() {
        // Send bang to output if it's active.
        if (this->find_bit(0)) {
            output_0.send("bang");
        }

        if (this->find_bit(1)) {
            output_1.send("bang");
        }

        if (this->find_bit(2)) {
            output_2.send("bang");
        }

        if (this->find_bit(3)) {
            output_3.send("bang");
        }

        if (this->find_bit(4)) {
            output_4.send("bang");
        }

        if (this->find_bit(5)) {
            output_5.send("bang");
        }

        if (this->find_bit(6)) {
            output_6.send("bang");
        }
    }

    void step() {
        // Add to the counter.
        counter++;

        // Handle the outputs.
        this->handle_output();
    }

    void reset_counter() {
        // Reset the counter.
        counter = -1;
    }

    // post to max window == but only when the class is loaded the first time
    message<> maxclass_setup{ this, "maxclass_setup",
        MIN_FUNCTION {
            return {};
        }
    };

    // define an optional argument for setting the message
    argument<symbol> bang_arg{ this, "bang_on", "Initial value for the greeting attribute.",
        MIN_ARGUMENT_FUNCTION {
            bang_enabled = TRUE;
        }
    };

    // the actual attribute for the message
    attribute<symbol> bang_on{ this, "bang_on", "symbol",
        description {
            "The output mode."
            "bool : boolean"
            "int  : integers"
        }
    };    

    // respond to the bang message to do something
    message<threadsafe::yes> bang{ this, "bang", "Steps the counter.",
        MIN_FUNCTION {
            switch (inlet) {
            case 0:
                //cout << "Inlet 0" << endl;
                this->step();
                break;
            case 1:
                //cout << "Inlet 1" << endl;
                this->reset_counter();
                break;
            default:
                cout << "Inlet Default" << endl;
                assert(false);
            }
            return {};
        }
    };

    // respond to the int message to do something
    message<threadsafe::yes> m_ints{ this, "int", "Steps the counter.",
        MIN_FUNCTION {
            switch (inlet) {
            case 0:
                //cout << "Inlet 0" << endl;
                if (args[0]) {
                    this->step();
                }
                break;
            case 1:
                //cout << "Inlet 1" << endl;
                if (args[0]) {
                    this->reset_counter();
                }
                break;
            default:
                cout << "Inlet Default" << endl;
                assert(false);
            }
            return {};
        }
    };

    // respond to the reset message to do something
    message<> reset{ this, "reset", "Reset the counter.",
        MIN_FUNCTION {
            this->reset_counter();

            return {};
        }
    };
};


MIN_EXTERNAL(CD4024);
