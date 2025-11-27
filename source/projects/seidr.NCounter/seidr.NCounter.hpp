/// @file       seidr.NCounter.hpp
///	@ingroup 	seidr
///	@copyright	Copyright 2025 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License found in the License.md file.

#pragma once

#include <c74_min.h>
#include "Counter/Counter.hpp"

#include <ext_mess.h>
#include <fcntl.h>

#include <string>
#include <vector>
#include <cstdint>

using namespace c74::min;

class NCounterMax : public object<NCounterMax> {
public:
    enum : uint8_t {
        OUTPUT_COUNT = 10
    };

    MIN_DESCRIPTION{"NCounter"};     // NOLINT 
    MIN_TAGS{"jb, counter"};         // NOLINT 
    MIN_AUTHOR{"Jóhann Berentsson"}; // NOLINT 
    MIN_RELATED{"seidr.*"};          // NOLINT 

    explicit NCounterMax(const atoms &args = {}) {};

    void handleOutputs();
    auto counterValue() -> unsigned int;
    auto preset() -> unsigned int;
    auto step() -> unsigned int;
    auto setPreset(int presetValue) -> unsigned int;

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

    std::array<outlet<>*, OUTPUT_COUNT> outputs = {&output0, &output1, &output2, &output3, &output4,
                                     &output5, &output6, &output7, &output8, &output9};
    
    argument<symbol> bangArg{this, "bang_on", "Initial value for the bang attribute.", MIN_ARGUMENT_FUNCTION{bangEnabled = FALSE; }};

    message<> bang{this, "bang", "Steps the counter.",
        MIN_FUNCTION{
            if (this->alreadyBanged){
                this->counter.step();
            } else {
                this->alreadyBanged = TRUE;
            }

            this->handleOutputs();
            return {};
        }
    };

    message<> reset{this, "reset", "Reset the counter.",
        MIN_FUNCTION{
            this->counter.reset();
            return {};
        }
    };

    message<> max_value{this, "max", "Set the counter max value.",
        MIN_FUNCTION{
            if(!args.empty()){
                this->counter.setMaxValue(static_cast<int> (args[0]));
            }
            return {};
        }
    };

private:
    Counter counter = Counter(NCounterMax::OUTPUT_COUNT);
    std::vector<int> outputStates_;
    bool bangEnabled = FALSE;
    bool alreadyBanged = FALSE;
};
