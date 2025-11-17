/// @file       seidr.BinaryCounter.hpp
///	@ingroup 	jb
///	@copyright	Copyright 2025 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License
/// found in the License.md file.

#pragma once

// #include <ext_mess.h>
// #include <fcntl.h>
#include <iostream>
#include <string>
#include <vector>

#include "c74_min.h"
#include "ShiftRegister/ShiftRegister.hpp"

using namespace c74::min;

#define BIT_COUNT 8
#define OUTPUT_COUNT 9
#define MAX_OUTPUTS 32

class LastNote {
private:
    uint64_t pitch = NULL;

public:
    bool dirty = FALSE;

    int get() { return this->pitch; }

    int set(int n) {
        this->pitch = n;

        if (this->dirty == FALSE) {
            this->dirty = TRUE;
        }

        return this->pitch;
    }
};

class ShiftRegister_MAX : public object<ShiftRegister_MAX> {
public:
    MIN_DESCRIPTION{"Shift Register"};
    MIN_TAGS{"jb, cmos"};
    MIN_AUTHOR{"Jóhann Berentsson"};
    MIN_RELATED{"seidr.*"};

    explicit ShiftRegister_MAX(const atoms& args = {});
    ~ShiftRegister_MAX() {};

    void handle_outputs();
    void handle_through();
    int size();
    int step();
    uint64_t get(int i);
    int data_input(int v);
    int data_through();

    inlet<> input_0{this, "(anything) input pulse"};
    inlet<> input_1{this, "(int|bang) input pulse"};
    inlet<> input_2{this, "(anything) input pulse"};

    std::vector<std::unique_ptr<outlet<>>> outputs;
    LastNote last_output[OUTPUT_COUNT] = {};

    c74::min::message<threadsafe::yes> anything{
        this, "anything", "Handle any message",
        MIN_FUNCTION{cout << "anything args size: " << args.size() << endl;
    return {};
}
}
;

c74::min::message<threadsafe::yes> symbol{
    this, "symbol", "Handle any message",
    MIN_FUNCTION{cout << "symbol args size: " << args.size() << endl;
return {};
}
}
;

c74::min::message<threadsafe::yes> bang{this, "bang", "step the shift register",
                                        MIN_FUNCTION{switch (inlet){case 0 : sr.step();
handle_through();

case 1:
break;

case 2:
sr.activate();
handle_outputs();

default:
cout << "Some other inlet: " << inlet << endl;
}

return {};
}
}
;

c74::min::message<threadsafe::yes> integer{
    this, "int", "data",
    MIN_FUNCTION{if (args.size()){switch (inlet){case 0 :

                                                     // sr.step();
                                                     // handle_outputs();
                                                     case 1 : this->sr.data_input(args[0]);

default:
std::cout << "Some other integer: " << args[0] << " Inlet: " << inlet << std::endl;
}
}

return {};
}
}
;

private:
ShiftRegister sr = ShiftRegister(BIT_COUNT);
bool every_output = TRUE;
bool send_bangs = FALSE;
int last_value = NULL;
}
;
