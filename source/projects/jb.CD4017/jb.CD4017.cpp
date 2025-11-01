/// @file       jb.CD4016.cpp
///	@ingroup 	jb
///	@copyright	Copyright 2022 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License found in the License.md file.

#include "c74_min.h"
#include <string>
#include <iostream>
#include <io.h>
#include <fcntl.h>

using namespace c74::min;


class CD4017 : public object<CD4017> {
public:
    MIN_DESCRIPTION	{"CMOS CD4016"};
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
    outlet<> output_7{ this, "(anything) output bit 7" };
    outlet<> output_8{ this, "(anything) output bit 8" };
    outlet<> output_9{ this, "(anything) output bit 9" };

    int counter = -1;
    int* counter_ptr = &counter;

    int is_active(int output) {
        // Is current output active?

        // Yes
        if (output == counter) {
            return 1;
        }

        // No
        return 0;
    }

    void send_output() {
        // Send data to the outputs.
        output_0.send(this->is_active(0));
        output_1.send(this->is_active(1));
        output_2.send(this->is_active(2));
        output_3.send(this->is_active(3));
        output_4.send(this->is_active(4));
        output_5.send(this->is_active(5));
        output_6.send(this->is_active(6));
        output_7.send(this->is_active(7));
        output_8.send(this->is_active(8));
        output_9.send(this->is_active(9));
    }

    void step() {
        // Add to the counter.
        counter++;

        // Reset to the first step.
        if (counter >= 10) {
            this->reset_counter();
        }

        this->send_output();
    }

    void reset_counter() {
        // Reset the counter.
        counter = 0;
    }

    // post to max window == but only when the class is loaded the first time
    message<> maxclass_setup{ this, "maxclass_setup",
        MIN_FUNCTION {
            return {};
        }
    };

    // define an optional argument for setting the message
    argument<symbol> output_type_arg{ this, "output_type", "Initial value for the greeting attribute.",
        MIN_ARGUMENT_FUNCTION {            
            output_type = arg;
            cout << "Argument: ";
            cout << arg << endl;
        }
    };

    // the actual attribute for the message
    attribute<symbol> output_type{ this, "output_type", "integer",
        description {
            "Greeting to be posted. "
            "The greeting will be posted to the Max console when a bang is received."
        }
    };

    // respond to the bang message to do something
    message<> bang{ this, "bang", "Steps the counter.",
        MIN_FUNCTION {
            cout << "Attribute: ";
            //cout << output_type << endl;
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


MIN_EXTERNAL(CD4017);
