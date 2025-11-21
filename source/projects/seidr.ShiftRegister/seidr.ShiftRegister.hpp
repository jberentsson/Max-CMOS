/// @file       seidr.BinaryCounter.hpp
///	@ingroup 	seidr
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
    bool dirty = false;

    [[nodiscard]] auto get() const -> uint64_t { return this->pitch_; }

    auto set(uint64_t noteValue) -> uint64_t {
        this->pitch_ = noteValue;

        if (!this->dirty) {
            this->dirty = true;
        }

        return this->pitch_;
    }
};

class ShiftRegisterMax : public object<ShiftRegisterMax> {
  public:
    MIN_DESCRIPTION{"Shift Register"}; // NOLINT 
    MIN_TAGS{"seidr"};              // NOLINT 
    MIN_AUTHOR{"Jóhann Berentsson"};   // NOLINT 
    MIN_RELATED{"seidr.*"};            // NOLINT 
    
	enum : uint8_t {
		BIT_COUNT = 8,
		OUTPUT_COUNT = 9,
		MAX_OUTPUTS = 32,
	};

    explicit ShiftRegisterMax(const atoms &args = {});

    void handleOutputs();
    void handleThrough();
    auto size() -> int;
    auto step() -> int;
    auto get(int index) -> uint64_t;
    auto dataInput(uint64_t value) -> uint64_t;
    auto dataThrough() -> uint64_t;

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
};

c74::min::message<threadsafe::yes> integer{
    this, "int", "data",
    MIN_FUNCTION{if (args.size()){switch (inlet){case 0 : // NOLINT 

                                                     // sr_.step();
                                                     // handleOutputs();
                                                     case 1 : this->sr_.dataInput(args[0]);

default:
std::cout << "Some other integer: " << args[0] << " Inlet: " << inlet << "\n";
}
}

        return {};
    }
};

private:
    ShiftRegister sr_ = ShiftRegister(BIT_COUNT);
    bool everyOutput = TRUE;
    bool sendBangs = FALSE;
    int lastValue_= NULL;
};
