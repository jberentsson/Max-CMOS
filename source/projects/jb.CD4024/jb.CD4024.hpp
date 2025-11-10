/// @file       jb.CD4024.hpp
///	@ingroup 	jb
///	@copyright	Copyright 2025 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License found in the License.md file.

#pragma once

#include "c74_max.h"
#include "c74_min.h"
#include <string>
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <ext_mess.h>

using namespace c74::min;

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

        void step();
        void reset_counter();
        void send_bangs();
        void handle_output();
        int find_bit(int output);

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

    private:
        int counter = -1;
        bool bang_enabled = FALSE;
};
