/// @file       seidr.Quantizer.hpp
///	@ingroup 	seidr
///	@copyright	Copyright 2025 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License
///             found in the License.md file.

#pragma once

#include "Quantizer/Quantizer.hpp"

#include <c74_min.h>
#include <ext_mess.h>
#include <fcntl.h>

using namespace c74::min;

class QuantizerMax : public object<QuantizerMax> {
public:
    MIN_DESCRIPTION{"Quantizer"};      // NOLINT 
    MIN_TAGS{"seidr"};                 // NOLINT 
    MIN_AUTHOR{"Jóhann Berentsson"};   // NOLINT 
    MIN_RELATED{"seidr.*"};            // NOLINT 

    //QuantizerMax() = default;
    explicit QuantizerMax(const atoms &args = {});

    c74::min::inlet<> input {this, "(int) input note"};
    c74::min::outlet<> output {this, "(int) output note"};

    c74::min::message<> anything {
        this, "int", "Process note messages",
        MIN_FUNCTION {
            if (!args.empty()){
                int note = args[0];
                output.send(quantizer.quantize(note));
            }
            return {};
        }
    };
    
private:
    Quantizer quantizer;
};
