/// @file       seidr.NCounter.hpp
///	@ingroup 	jb
///	@copyright	Copyright 2025 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License
///found in the License.md file.

#pragma once

#include "NCounter/NCounter.hpp"
#include "c74_min.h"
#include <ext_mess.h>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <vector>

using namespace c74::min;

#define OUTPUT_COUNT 10

class NCounter_MAX : public object<NCounter_MAX> {
public:
  MIN_DESCRIPTION{"NCounter"};
  MIN_TAGS{"jb, counter"};
  MIN_AUTHOR{"Jóhann Berentsson"};
  MIN_RELATED{"seidr.*"};

  explicit NCounter_MAX(const atoms &args = {}) {};
  ~NCounter_MAX() {};

  void handle_outputs();
  int counter_value();
  int preset();
  int step();
  int set_preset(int p);

  inlet<> input_0{this, "(bang) input pulse"};
  inlet<> input_1{this, "(reset) reset pulse"};

  outlet<> output_0{this, "(anything) output bit 0"};
  outlet<> output_1{this, "(anything) output bit 1"};
  outlet<> output_2{this, "(anything) output bit 2"};
  outlet<> output_3{this, "(anything) output bit 3"};
  outlet<> output_4{this, "(anything) output bit 4"};
  outlet<> output_5{this, "(anything) output bit 5"};
  outlet<> output_6{this, "(anything) output bit 6"};
  outlet<> output_7{this, "(anything) output bit 7"};
  outlet<> output_8{this, "(anything) output bit 8"};
  outlet<> output_9{this, "(anything) output bit 9"};

  outlet<> *outputs[10] = {&output_0, &output_1, &output_2, &output_3,
                           &output_4, &output_5, &output_6, &output_7,
                           &output_8, &output_9};

  argument<symbol> bang_arg{this, "bang_on",
                            "Initial value for the bang attribute.",
                            MIN_ARGUMENT_FUNCTION{bang_enabled = FALSE;
}
}
;

argument<symbol> output_type_arg{this, "output_type",
                                 "Initial value for the greeting attribute.",
                                 MIN_ARGUMENT_FUNCTION{output_type = arg;
}
}
;

attribute<symbol> output_type{
    this, "output_type", "integer",
    description{"Greeting to be posted. "
                "The greeting will be posted to the Max console when a bang is "
                "received."}};

message<> bang{this, "bang", "Steps the counter.",
               MIN_FUNCTION{if (this->already_banged){this->counter.step();
}
else {
  this->already_banged = TRUE;
}

this->handle_outputs();
return {};
}
}
;

message<> reset{this, "reset", "Reset the counter.",
                MIN_FUNCTION{this->counter.reset();
return {};
}
}
;

private:
NCounter counter = NCounter(OUTPUT_COUNT);
std::vector<int> output_states;
bool bang_enabled = FALSE;
bool already_banged = FALSE;
}
;
