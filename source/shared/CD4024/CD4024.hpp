#pragma once

#include "../Counter/Counter.hpp"

class CD4024 : public Counter {
	public:
		CD4024();
		CD4024(int n);

		int get_max_value();
		int set_max_value(int n);

		using Counter::step;
		using Counter::reset;
		using Counter::enable;
		using Counter::value;
		using Counter::direction;
		//using Counter::set_max_value;
};
