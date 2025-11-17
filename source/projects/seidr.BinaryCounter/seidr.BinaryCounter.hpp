/// @file       seidr.BinaryCounter.hpp
///	@ingroup 	jb
///	@copyright	Copyright 2022 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License
/// found in the License.md file.

#pragma once

#include "BinaryCounter/BinaryCounter.hpp"
#include "c74_min.h"
#include <ext_mess.h>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <vector>

using namespace c74::min;

#define OUTPUT_COUNT 8

class BinaryCounter_MAX : public object<BinaryCounter_MAX> {
  public:
    MIN_DESCRIPTION{"Binary Counter"};
    MIN_TAGS{"jb, counter"};
    MIN_AUTHOR{"Jóhann Berentsson"};
    MIN_RELATED{"seidr.*"};

    explicit BinaryCounter_MAX(const atoms &args = {});
    ~BinaryCounter_MAX() {};

    void enable_bangs();
    void disable_bangs();

    void update_outputs();
    int get_bit(int output);

    int counter_value();
    int set_preset(int p);
    int preset();
    int max_value();

    inlet<> input_0{this, "(bang | list | reset) input pulse"};
    inlet<> input_1{this, "(reset) reset pulse"};

    std::vector<std::unique_ptr<outlet<>>> outputs;

    argument<symbol> bang_arg {
        this, "bang",
            "Initial value for the bang
            attribute.", MIN_ARGUMENT_FUNCTION {bang_enabled = FALSE;}};

            attribute<symbol>
                bang_on{this, "bang", "symbol",
                        description{"The output mode."
                                    "bool : boolean"
                                    "int  : integers"}};

        message<threadsafe::yes> bang{
            this, "bang", "Steps the counter.",
            MIN_FUNCTION{if (this->already_banged){
                if (this->reset_triggered){this->counter.reset();
        			this->reset_triggered = FALSE;
    			}
			    else {
       	 			this->counter.step();
    			}
				} else {
    			this->already_banged = TRUE;
}

this->update_outputs();
return {};
}
}
;


message<threadsafe::yes> list{this, "list", "Handle any list.",
                              MIN_FUNCTION{std::cout << "LIST!" << std::endl;

if (args.size() >= 1) {
    for (int i = 0; i < args.size(); i++) {
        std::cout << "THE ARG: " << args[i] << std::endl;
    }
}

return {};
}
}
;

message<threadsafe::yes> reset{this, "reset", "Reset the counter.",
                               MIN_FUNCTION{this->reset_triggered = TRUE;
return {};
}
}
;

message<threadsafe::yes> anything{
    this, "anything", "Handle any message.",
    MIN_FUNCTION{std::cout << "ANYTHING!" << std::endl;
return {};}};

private:
BinaryCounter counter = BinaryCounter(OUTPUT_COUNT);
bool bang_enabled = FALSE;
bool already_banged = FALSE;
bool reset_triggered = FALSE;
};
