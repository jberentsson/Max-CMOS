/// @file       jb.BinaryCounter.hpp
///	@ingroup 	jb
///	@copyright	Copyright 2022 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License found in the License.md file.

#pragma once

#include "c74_min.h"
#include <string>
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <ext_mess.h>
#include <vector>
#include "BinaryCounter/BinaryCounter.hpp"

using namespace c74::min;

#define OUTPUT_COUNT 8

class BinaryCounter_MAX : public object<BinaryCounter_MAX> {
    public:
        MIN_DESCRIPTION	{"Binary Counter"};
        MIN_TAGS		{"jb, counter"};
        MIN_AUTHOR		{"Jóhann Berentsson"};
        MIN_RELATED		{"jb.*"};
        
        BinaryCounter_MAX(const atoms& args = {});
        ~BinaryCounter_MAX(){};

        void enable_bangs();
        void disable_bangs();

        void update_outputs();
        int get_bit(int output);

        int counter_value();
        int set_preset(int p);
        int preset();
        int max_value();

        inlet<>  input_0 { this, "(bang | list | reset) input pulse" };
        inlet<>  input_1 { this, "(reset) reset pulse" };

        outlet<> output_0 { this, "(anything) output bit 0" };
        outlet<> output_1 { this, "(anything) output bit 1" };
        outlet<> output_2 { this, "(anything) output bit 2" };
        outlet<> output_3 { this, "(anything) output bit 3" };
        outlet<> output_4 { this, "(anything) output bit 4" };
        outlet<> output_5 { this, "(anything) output bit 5" };
        outlet<> output_6 { this, "(anything) output bit 6" };
        outlet<> output_7 { this, "(anything) output bit 6" };

        outlet<> *outputs[OUTPUT_COUNT] = {
            &output_0,
            &output_1,
            &output_2,
            &output_3,
            &output_4,
            &output_5,
            &output_6,
            &output_7
        };

        argument<symbol> bang_arg{ this, "bang_on", "Initial value for the bang attribute.",
            MIN_ARGUMENT_FUNCTION {
                bang_enabled = FALSE;
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

        message<threadsafe::yes> list { this, "list", "Handle any list.",
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

        message<threadsafe::yes> reset { this, "reset", "Reset the counter.",
            MIN_FUNCTION {
                this->reset_triggered = TRUE;
                
                return {};
            }
        };

        message<threadsafe::yes> anything { this, "anything", "Handle any message.",
            MIN_FUNCTION {
                std::cout << "ANYTHING!" << std::endl;
                return {};
            }
        };

    private:
        BinaryCounter counter = BinaryCounter(OUTPUT_COUNT);
        
        bool bang_enabled = FALSE;
        bool already_banged = FALSE;
        bool reset_triggered = FALSE;
};
