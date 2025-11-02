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

    outlet<> *outputs[7] = {
        &output_0,
        &output_1,
        &output_2,
        &output_3,
        &output_4,
        &output_5,
        &output_6
    }; 

    int counter = -1;
    int* counter_ptr = &counter;
    bool bang_enabled = FALSE;

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
            for (int i = 0; i < 7; i++){
                outputs[i]->send(this->find_bit(i));
            }
        }
    }

    void send_bangs() {
        for (int i = 0; i < 7; i++){
            if (this->find_bit(i)) {
                outputs[i]->send("bang");
            }
        }
    }

    void step() {
        counter++;
        this->handle_output();
    }

    void reset_counter() {
        counter = 0;
    }

    argument<symbol> bang_arg{ this, "bang_on", "Initial value for the bang attribute.",
        MIN_ARGUMENT_FUNCTION {
            bang_enabled = TRUE;
        }
    };

    attribute<symbol> bang_on{ this, "bang_on", "symbol",
        description {
            "The output mode."
            "bool : boolean"
            "int  : integers"
        }
    };

    message<threadsafe::yes> bang{ this, "bang", "Steps the counter.",
        MIN_FUNCTION {
            this->step();
            return {};
        }
    };

    message<threadsafe::yes> reset{ this, "reset", "Reset the counter.",
        MIN_FUNCTION {
            this->reset_counter();
            return {};
        }
    };
};

MIN_EXTERNAL(CD4024);
