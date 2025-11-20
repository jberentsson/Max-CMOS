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
#include <cstdint>

using namespace c74::min;

class BinaryCounterMax : public object<BinaryCounterMax> {
public:
    MIN_DESCRIPTION{"Binary Counter"}; // NOLINT 
    MIN_TAGS{"jb, counter"};           // NOLINT 
    MIN_AUTHOR{"Jóhann Berentsson"};   // NOLINT 
    MIN_RELATED{"seidr.*"};            // NOLINT 
    
    enum : std::uint8_t {
        OUTPUT_COUNT = 8
    };

    explicit BinaryCounterMax(const atoms &args = {});
    ~BinaryCounterMax() = default;

    void enableBangs();
    void disableBangs();
    void updateOutputs();
    unsigned int getBit(int output);

    unsigned int counterValue();
    unsigned int setPreset(unsigned int p);
    unsigned int preset();
    unsigned int maxValue();

    inlet<> input0 {this, "(bang | list | reset) input pulse"};
    inlet<> input1 {this, "(reset) reset pulse"};

    std::vector<std::unique_ptr<outlet<>>> outputs;

    // FIXED: Proper attribute for bang mode
    attribute<bool> bangEnable{this, "bangEnable", false,
        description{"Output mode: true for bang outputs, false for integer outputs"}};

    // FIXED: Simplified bang handler
    message<threadsafe::yes> bang{
        this, "bang", "Steps the counter.",
        MIN_FUNCTION{
            this->counter_.step();
            this->updateOutputs();
            return {};
        }
    };

    // FIXED: Reset handler that updates outputs
    message<threadsafe::yes> reset{
        this, "reset", "Reset the counter.",
        MIN_FUNCTION{
            this->counter_.reset();
            this->updateOutputs();  // CRITICAL: Update outputs after reset
            return {};
        }
    };

    // FIXED: Preset handler
    //message<threadsafe::yes> preset_msg{
    //    this, "preset", "Set preset value.",
    //    MIN_FUNCTION{
    //        if (args.size() > 0) {
    //            int preset_value = args[0];
    //            this->counter_.setPreset(preset_value);
    //            this->updateOutputs();  // Update outputs after preset change
    //        }
    //        return {};
    //    }
    //};

    // FIXED: Set value handler
    message<threadsafe::yes> set{
        this, "set", "Set counter value.",
        MIN_FUNCTION{
            if (args.size() > 0) {
                int value = args[0];
                // You'll need to implement a setValue method in BinaryCounter
                //this->counter_.setValue(value);
                this->updateOutputs();  // Update outputs after value change
            }
            return {};
        }
    };

    // FIXED: Output current value
    message<threadsafe::yes> output{
        this, "output", "Output current value without changing it.",
        MIN_FUNCTION{
            this->updateOutputs();
            return {};
        }
    };

private:
    BinaryCounter counter_ = BinaryCounter(OUTPUT_COUNT);
};
