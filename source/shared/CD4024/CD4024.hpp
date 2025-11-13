#pragma once

#include "../Counter/Counter.hpp"

class CD4024 : public Counter {
	public:
		CD4024(int n);

		using Counter::step;
		using Counter::reset;
		using Counter::enable;
		using Counter::value;
		using Counter::direction;
};
