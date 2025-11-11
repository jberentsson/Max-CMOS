#pragma once

#include "../Counter/Counter.hpp"

class CD4017 : public Counter {
	private:
		using Counter::forward;
		using Counter::back;
		using Counter::preset;
		using Counter::direction;
		using Counter::set;

	public:
		CD4017();
		CD4017(int n);
		int step();
};
