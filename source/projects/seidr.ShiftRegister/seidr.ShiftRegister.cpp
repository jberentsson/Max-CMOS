/// @file       seidr.ShiftRegister.cpp
///	@ingroup 	jb
///	@copyright	Copyright 2018 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License found in the License.md file.

#include <iostream>

#include "c74_min.h"
#include "seidr.ShiftRegister.hpp"

using namespace c74::min;

ShiftRegister_MAX::ShiftRegister_MAX(const atoms& args){
    int number_of_outputs = 0;

    if (args.size() > 0) {
        number_of_outputs = args[0];        
    } else {
        number_of_outputs = OUTPUT_COUNT;
    }

    for (int i = 0; i < number_of_outputs; i++) {
        outputs.push_back(
            std::make_unique<outlet<>>(this, "( int | bang ) output " + std::to_string(i))
        );
    }

    for(int i = 0; i < args.size(); i++){
        cout << "arg[" << i << "]" << args[i] << endl;
    }
};

void ShiftRegister_MAX::handle_outputs() {
    // Bit outputs from 0 to (N-1).
    for (int i = 0; i < outputs.size() - 1; i++){
        this->outputs[i]->send(this->send_bangs ? bang() : atoms({(uint64_t) this->sr.get(i)}));
    }
}

void ShiftRegister_MAX::handle_through() {
    // Output N data through.
    uint64_t dt = this->sr.data_through();
    int last_output_index = outputs.size() - 1;

    if (every_output || dt != last_output[8].get()){
        this->outputs[last_output_index]->send(this->send_bangs ? bang() : c74::min::atoms({dt}));
        this->last_output[last_output_index].set(dt);
    }
}

int ShiftRegister_MAX::size() {
    return this->sr.size();
}

int ShiftRegister_MAX::step(){
    return this->sr.step();
}

uint64_t ShiftRegister_MAX::get(int i){
    return this->sr.get(i);
}

int ShiftRegister_MAX::data_input(int v){
    return this->sr.data_input(v);
}

int ShiftRegister_MAX::data_through(){
    return this->sr.data_through();
}

MIN_EXTERNAL(ShiftRegister_MAX);
