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
#include <iostream>

using namespace c74::min;

class QuantizerMax : public c74::min::object<QuantizerMax> {
  public:

    MIN_DESCRIPTION{"Quantizer"};      // NOLINT 
    MIN_TAGS{"seidr"};                 // NOLINT 
    MIN_AUTHOR{"Jóhann Berentsson"};   // NOLINT 
    MIN_RELATED{"seidr.*"};            // NOLINT 

    explicit QuantizerMax(const atoms &args = {});
    auto processNote(int note, int velocity) -> void;
    auto addNote(int noteValue) -> int { return this->quantizer.addNote(noteValue); }
    auto noteCount() -> int { return this->quantizer.noteCount(); }
    auto getRoundDirection() -> Quantizer::RoundDirection { return this->quantizer.getRoundDirection(); }
    auto setRoundDirection(Quantizer::RoundDirection direction) -> Quantizer::RoundDirection { return this->quantizer.setRoundDirection(direction); }
    auto setMode(Quantizer::QuantizeMode mode) -> Quantizer::QuantizeMode { return this->quantizer.setMode(mode); }

    c74::min::inlet<> input {this, "(anything) input note"};

    c74::min::outlet<> output0 {this, "(anything) output note"};
    c74::min::outlet<> output1 {this, "(anything) output velocity"};

    c74::min::message<> anything {
        this, "anything", "Process note messages",
        MIN_FUNCTION {
            if (!args.empty()){
                cout << "ANYTHING: Arg.size(): " << args.size() << endl;
            }
            return {};
        }
    };
    
    c74::min::message<> integer {
        this, "integer", "Process note messages",
            MIN_FUNCTION {
            if (!args.empty()) {
                int note = args[0];
                this->processNote(note, MIDI::RANGE_HIGH + 1);
            }
            return {};
        }
    };

    c74::min::message<> list{
        this, "list", "Process note messages",
        MIN_FUNCTION{
            if (!args.empty() && args.size() == 2){
                int note = args[0];
                int velocity = args[1];
                this->processNote(note, velocity);
            }
            return {};
        }
    };

    c74::min::message<> add{
        this, "add", "Process note messages",
        MIN_FUNCTION{
            if (!args.empty()) {
                for (const auto &arg : args) {
                    int note = arg;
                    if(note >= MIDI::RANGE_LOW && note <= MIDI::RANGE_HIGH ) {
                        this->quantizer.addNote(note);
                    }
                }                
            }
            return {};
        }
    };

    c74::min::message<> del{
        this, "del", "Process note messages",
        MIN_FUNCTION{
            if (!args.empty()){
                for(const auto &arg: args){
                    this->quantizer.deleteNote(arg);
                }
            }
            return {};
        }
    };

private:
    Quantizer quantizer;
};

MIN_EXTERNAL(QuantizerMax); // NOLINT
