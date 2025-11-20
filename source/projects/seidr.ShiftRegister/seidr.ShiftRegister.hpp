/// @file       seidr.BinaryCounter.hpp
///	@ingroup 	jb
///	@copyright	Copyright 2025 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License
/// found in the License.md file.

#pragma once

#include "c74_min.h"
#include "ShiftRegister/ShiftRegister.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

using namespace c74::min;

class LastNote {
  private:
  	uint64_t pitch_ = NULL;

  public:
    bool dirty = FALSE;

    [[nodiscard]] uint64_t get() const { return this->pitch_; }

    uint64_t set(uint64_t n) {
        this->pitch_ = n;

        if (this->dirty == FALSE) {
            this->dirty = TRUE;
        }

        return this->pitch_;
    }
};

class ShiftRegisterMax : public object<ShiftRegisterMax> {
  public:
    MIN_DESCRIPTION{"Shift Register"}; // NOLINT 
    MIN_TAGS{"jb, cmos"};              // NOLINT 
    MIN_AUTHOR{"Jóhann Berentsson"};   // NOLINT 
    MIN_RELATED{"seidr.*"};            // NOLINT 
    
	enum : std:uint8_t {
		BIT_COUNT = 8,
		OUTPUT_COUNT = 9,
		MAX_OUTPUTS = 32,
	};

    explicit ShiftRegisterMax(const atoms &args = {});
    ~ShiftRegisterMax() = default;

    void handleOutputs();
    void handleThrough();
    int size();
    int step();
    uint64_t get(int i);
    int dataInput(int v);
    int dataThrough();

    inlet<> input0{this, "(anything) input pulse"};
    inlet<> input1{this, "(int|bang) input pulse"};
    inlet<> input2{this, "(anything) input pulse"};

    std::vector<std::unique_ptr<outlet<>>> outputs;
    LastNote lastOutput[OUTPUT_COUNT] = {};

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
                                        MIN_FUNCTION{switch (inlet){case 0 : sr_.step();
handleThrough();

case 1:
break;

case 2:
sr_.activate();
handleOutputs();

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

                                                     // sr_.step();
                                                     // handleOutputs();
                                                     case 1 : this->sr_.dataInput(args[0]);

default:
std::cout << "Some other integer: " << args[0] << " Inlet: " << inlet << std::endl;
}
}

return {};
}
}
;

private:
ShiftRegister sr_= ShiftRegister(BIT_COUNT);
bool everyOutput = TRUE;
bool sendBangs = FALSE;
int lastValue_= NULL;
}
;
