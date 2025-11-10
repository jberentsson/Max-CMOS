#pragma once

#include "Counter.hpp"

class CD4017 : private Counter {
	private:
		bool isEnabled = false;

	public:
		CD4017(int n);
		int enable();
		int step();
};
