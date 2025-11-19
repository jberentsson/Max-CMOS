/// @file       seidr.BinaryCounter.hpp
///	@ingroup 	jb
///	@copyright	Copyright 2022 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License
/// found in the License.md file.

#pragma once

#include "c74_min.h"

#include "BinaryCounter/BinaryCounter.hpp"

#include <ext_mess.h>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <vector>

using namespace c74::min;

class BinaryCounterMax : public object<BinaryCounterMax> {
  public:
    MIN_DESCRIPTION{"Binary Counter"};
    MIN_TAGS{"jb, counter"};
    MIN_AUTHOR{"Jóhann Berentsson"};
    MIN_RELATED{"seidr.*"};

    explicit BinaryCounterMax(const atoms &args = {});
    ~BinaryCounterMax() = default;

    void enableBangs();
    void disableBangs();

    void updateOutputs();
    int getBit(int output);

    int counterValue();
    int setPreset(int p);
    int preset();
    int maxValue();

    inlet<> input0 {this, "(bang | list | reset) input pulse"};
    inlet<> input1 {this, "(reset) reset pulse"};

    std::vector<std::unique_ptr<outlet<>>> outputs;

    argument<symbol> bang_arg{this, "bang", "Initial value for the bang attribute.",
                              MIN_ARGUMENT_FUNCTION{bangEnable = FALSE;
}
}
;

attribute<symbol> bang_on{this, "bang", "symbol",
                          description{"The output mode."
                                      "bool : boolean"
                                      "int  : integers"}};

message<threadsafe::yes> bang{
    this, "bang", "Steps the counter.",
    MIN_FUNCTION{if (this->alreadyBanged){if (this->resetTriggered){this->counter.reset();
this->resetTriggered = FALSE;
}
else {
    this->counter.step();
}
}
else {
    this->alreadyBanged = TRUE;
}

this->updateOutputs();
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
                               MIN_FUNCTION{this->resetTriggered = TRUE;
return {};
}
}
;

message<threadsafe::yes> anything{this, "anything", "Handle any message.",
                                  MIN_FUNCTION{std::cout << "ANYTHING!" << std::endl;
return {};
}
}
;

private:
BinaryCounter counter_ = BinaryCounter(OUTPUT_COUNT);
bool bangEnable = FALSE;
bool alreadyBanged = FALSE;
bool resetTriggered = FALSE;

enum {
    OUTPUT_COUNT = 8
};

}
;
