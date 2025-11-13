#pragma once

#include "../Counter/Counter.hpp"

class CD4024 : public Counter {
	public:
		CD4024();
		CD4024(int n);
};
