#pragma once

#include "../Counter/Counter.hpp"

class CD4017 : public Counter {
	public:
		CD4017(int n);

		using Counter::step;
		using Counter::reset;
		using Counter::enable;
		using Counter::value;
};
