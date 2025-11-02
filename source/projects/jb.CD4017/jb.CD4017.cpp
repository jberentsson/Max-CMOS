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

    outlet<> *outputs[10] = {
        &output_0,
        &output_1,
        &output_2,
        &output_3,
        &output_4,
        &output_5,
        &output_6,
        &output_7,
        &output_8,
        &output_9
    }; 

    int counter = -1;
    int* counter_ptr = &counter;

    void send_output() {
        // Send data to the outputs.
        for (int i = 0; i < 10; i++){
            outputs[i]->send(i == counter);
        }
    }

    void step() {
        counter++;

        if (counter >= 10) {
            this->reset_counter();
        }

        this->send_output();
    }

    void reset_counter() {
        counter = -1;
    }

    argument<symbol> output_type_arg{ this, "output_type", "Initial value for the greeting attribute.",
        MIN_ARGUMENT_FUNCTION {            
            output_type = arg;
        }
    };

    attribute<symbol> output_type{ this, "output_type", "integer",
        description {
            "Greeting to be posted. "
            "The greeting will be posted to the Max console when a bang is received."
        }
    };

    message<> bang{ this, "bang", "Steps the counter.",
        MIN_FUNCTION {
            this->step();
            return {};
        }
    };

    message<> reset{ this, "reset", "Reset the counter.",
        MIN_FUNCTION {
            this->reset_counter();
            return {};
        }
    };
};

MIN_EXTERNAL(CD4017);
