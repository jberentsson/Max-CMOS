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
#include "CD4024/CD4024.hpp"

using namespace c74::min;

#define OUTPUT_COUNT 8

class CD4024_MAX : public c74::min::object<CD4024_MAX> {
    public:
        MIN_DESCRIPTION	{"CMOS CD4024"};
        MIN_TAGS		{"cmos"};
        MIN_AUTHOR		{"Jóhann Berentsson"};
        MIN_RELATED		{"print, jit.print, dict.print"};

        void enable_bangs();
        void disable_bangs();

        void update_outputs();
        int get_bit(int output);

        int counter_value();
        int set_preset(int p);
        int preset();
        int max_value();

        c74::min::inlet<>  input_0 { this, "(bang | list | reset) input pulse" };
        c74::min::inlet<>  input_1 { this, "(reset) reset pulse" };

        c74::min::outlet<> output_0 { this, "(anything) output bit 0" };
        c74::min::outlet<> output_1 { this, "(anything) output bit 1" };
        c74::min::outlet<> output_2 { this, "(anything) output bit 2" };
        c74::min::outlet<> output_3 { this, "(anything) output bit 3" };
        c74::min::outlet<> output_4 { this, "(anything) output bit 4" };
        c74::min::outlet<> output_5 { this, "(anything) output bit 5" };
        c74::min::outlet<> output_6 { this, "(anything) output bit 6" };
        c74::min::outlet<> output_7 { this, "(anything) output bit 6" };

        c74::min::outlet<> *outputs[OUTPUT_COUNT] = {
            &output_0,
            &output_1,
            &output_2,
            &output_3,
            &output_4,
            &output_5,
            &output_6,
            &output_7
        };

        c74::min::argument<c74::min::symbol> bang_arg{ this, "bang_on", "Initial value for the bang attribute.",
            MIN_ARGUMENT_FUNCTION {
                bang_enabled = FALSE;
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
                if (this->already_banged) {
                    if(this->reset_triggered){
                        this->counter.reset();
                        this->reset_triggered = FALSE;
                    } else {
                        this->counter.step();
                    }
                } else {
                    this->already_banged = TRUE;
                }

                this->update_outputs();
                return {};
            }
        };

        //c74::min::message<c74::min::threadsafe::yes> reset { this, "reset", "Reset the counter.",
        //    MIN_FUNCTION {
        //        this->reset_triggered = TRUE;
        //        return {};
        //    }
        //};

        c74::min::message<> list { this, "list", "Handle any message",
            MIN_FUNCTION {
                std::cout << "LIST!" << std::endl;
                if (args.size() >= 1) {
                    for(int i = 0; i < args.size(); i++){
                        std::cout << "THE ARG: " << args[i] << std::endl;
                    }
                }
                
                return {};
            }
        };

        c74::min::message<> reset { this, "reset", "Handle any message",
            MIN_FUNCTION {
                this->reset_triggered = TRUE;
                
                return {};
            }
        };

        c74::min::message<> anything { this, "anything", "Handle any message",
            MIN_FUNCTION {
                std::cout << "ANYTHING!" << std::endl;
                return {};
            }
        };

    private:
        CD4024 counter = CD4024(OUTPUT_COUNT);
        bool bang_enabled = FALSE;
        bool already_banged = FALSE;
        bool reset_triggered = FALSE;
};
