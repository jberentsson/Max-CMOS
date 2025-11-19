/// @file       seidr.NCounter.hpp
///	@ingroup 	jb
///	@copyright	Copyright 2025 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License
/// found in the License.md file.

#pragma once

#include "c74_min.h"

#include "NCounter/NCounter.hpp"

#include <ext_mess.h>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <vector>

using namespace c74::min;

class NCounterMax : public object<NCounterMax> {
  public:
    enum {
        OUTPUT_COUNT = 10
    };

    MIN_DESCRIPTION{"NCounter"};
    MIN_TAGS{"jb, counter"};
    MIN_AUTHOR{"Jóhann Berentsson"};
    MIN_RELATED{"seidr.*"};

    explicit NCounterMax(const atoms &args = {}) {};
    ~NCounterMax() = default;

    void handleOutputs();
    int counterValue();
    int preset();
    int step();
    int set_preset(int p);

    inlet<> input0{this, "(bang) input pulse"};
    inlet<> input1{this, "(reset) reset pulse"};

    outlet<> output0{this, "(anything) output bit 0"};
    outlet<> output1{this, "(anything) output bit 1"};
    outlet<> output2{this, "(anything) output bit 2"};
    outlet<> output3{this, "(anything) output bit 3"};
    outlet<> output4{this, "(anything) output bit 4"};
    outlet<> output5{this, "(anything) output bit 5"};
    outlet<> output6{this, "(anything) output bit 6"};
    outlet<> output7{this, "(anything) output bit 7"};
    outlet<> output8{this, "(anything) output bit 8"};
    outlet<> output9{this, "(anything) output bit 9"};

    outlet<> *outputs[10] = {&output0, &output1, &output2, &output3, &output4,
                             &output5, &output6, &output7, &output8, &output9};

    argument<symbol> bang_arg{this, "bang_on", "Initial value for the bang attribute.",
                              MIN_ARGUMENT_FUNCTION{bangEnabled = FALSE;
}
}
;

argument<symbol> outputType_arg{this, "outputType", "Initial value for the greeting attribute.",
                                 MIN_ARGUMENT_FUNCTION{outputType = arg;
}
}
;

attribute<symbol> outputType{
    this, "outputType", "integer",
    description{"Greeting to be posted. "
                "The greeting will be posted to the Max console when a bang is "
                "received."}};

message<> bang{this, "bang", "Steps the counter.",

               MIN_FUNCTION{if (this->alreadyBanged){this->counter.step();
}
else {
    this->alreadyBanged = TRUE;
}

this->handleOutputs();
return {};
}
}
;

message<> reset{this, "reset", "Reset the counter.", MIN_FUNCTION{this->counter.reset();
return {};
}
}
;

private:
NCounter counter_ = NCounter(OUTPUT_COUNT);
std::vector<int> outputStates_;
bool bangEnabled = FALSE;
bool alreadyBanged = FALSE;
}
;
