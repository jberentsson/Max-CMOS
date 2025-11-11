#pragma once

#include "../Counter/Counter.hpp"

class CD4024 : public Counter {
	private:
		using Counter::forward;
		using Counter::back;

	public:
		CD4024();
		CD4024(int n);
};
