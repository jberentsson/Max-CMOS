/// @file       jb.CD4024.cpp
///	@ingroup 	jb
///	@copyright	Copyright 2018 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License found in the License.md file.

#include "c74_max.h"
#include "c74_min.h"
#include <string>
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <ext_mess.h>

//using namespace c74::min;
using namespace c74::max;

class CD4024 : public c74::min::object<CD4024> {
public:
    MIN_DESCRIPTION	{"CMOS CD4024"};
    MIN_TAGS		{"cmos"};
    MIN_AUTHOR		{"Jóhann Berentsson"};
    MIN_RELATED		{"print, jit.print, dict.print"};

    c74::min::inlet<>  input_0 { this, "(bang) input pulse" };
    c74::min::inlet<>  input_1 { this, "(reset) reset pulse" };

    c74::min::outlet<> output_0 { this, "(anything) output bit 0" };
    c74::min::outlet<> output_1 { this, "(anything) output bit 1" };
    c74::min::outlet<> output_2 { this, "(anything) output bit 2" };
    c74::min::outlet<> output_3 { this, "(anything) output bit 3" };
    c74::min::outlet<> output_4 { this, "(anything) output bit 4" };
    c74::min::outlet<> output_5 { this, "(anything) output bit 5" };
    c74::min::outlet<> output_6 { this, "(anything) output bit 6" };

    c74::min::outlet<> *outputs[7] = {
        &output_0,
        &output_1,
        &output_2,
        &output_3,
        &output_4,
        &output_5,
        &output_6
    }; 

    int counter = -1;
    bool bang_enabled = FALSE;

    int find_bit(int output) {
        // isolate the correct bit.
        return ((this->counter) >> output) & 0x1;
    }

    void handle_output() {
        // Send data to the outputs
        // TODO: Send only data on change

        if (this->bang_enabled) {
            this->send_bangs();
        } else {
            for (int i = 0; i < 7; i++){
                this->outputs[i]->send(this->find_bit(i));
            }
        }
    }

    void send_bangs() {
        for (int i = 0; i < 7; i++){
            if (this->find_bit(i)) {
                this->outputs[i]->send("bang");
            }
        }
    }

    void step() {
        this->counter++;
        this->handle_output();
    }

    void reset_counter() {
        this->counter = 0;
    }

    c74::min::argument<c74::min::symbol> bang_arg{ this, "bang_on", "Initial value for the bang attribute.",
        MIN_ARGUMENT_FUNCTION {
            bang_enabled = TRUE;
        }
    };

    c74::min::attribute<c74::min::symbol> bang_on{ this, "bang_on", "symbol",
        c74::min::description {
            "The output mode."
            "bool : boolean"
            "int  : integers"
        }
    };

    c74::min::message<c74::min::threadsafe::yes> bang{ this, "bang", "Steps the counter.",
        MIN_FUNCTION {
            this->step();
            return {};
        }
    };

    c74::min::message<c74::min::threadsafe::yes> reset{ this, "reset", "Reset the counter.",
        MIN_FUNCTION {
            this->reset_counter();
            return {};
        }
    };
};

MIN_EXTERNAL(CD4024);
