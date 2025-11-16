/// @file       seidr.ShiftRegister.cpp
///	@ingroup 	jb
///	@copyright	Copyright 2018 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License found in the License.md file.

#include <iostream>

#include "c74_min.h"
#include "seidr.ShiftRegister.hpp"

using namespace c74::min;

ShiftRegister_MAX::ShiftRegister_MAX(const atoms& args){
    std::cout << args.size() << std::endl;

    for(int i = 0; i < args.size(); i++){
        cout << "arg[" << i << "]" << args[i] << endl;
    }
};

void ShiftRegister_MAX::handle_outputs(){
    // Bit outputs 0-7.
    for (int i = 0; i < BIT_COUNT; i++){
        if (this->send_bangs){
            this->outputs[i]->send(bang());
        } else {
            this->outputs[i]->send(this->sr.get(i));
        }
    }
}

void ShiftRegister_MAX::handle_through(){
    // Data through.
    int dt = this->sr.data_through();

    if (every_output || dt != last_output[8]){
        if(this->send_bangs){
            this->output_8.send(bang());
        } else {
            this->output_8.send(dt);
        }
        this->last_output[8] = dt;
    }
}

int ShiftRegister_MAX::size(){
    return this->sr.size();
}

int ShiftRegister_MAX::step(){
    return this->sr.step();
}

int ShiftRegister_MAX::get(int i){
    return this->sr.get(i);
}

int ShiftRegister_MAX::data_input(int v){
    return this->sr.data_input(v);
}

int ShiftRegister_MAX::data_through(){
    return this->sr.data_through();
}

MIN_EXTERNAL(ShiftRegister_MAX);
