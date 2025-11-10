#include "CD4017.hpp"

CD4017::CD4017(int n) {
    this->steps = n;
}

int CD4017::enable() {
	this->isEnabled = !this->isEnabled;

	return isEnabled;
}

int CD4017::step() {
	if(this->isEnabled) {
		return this->counter++;
	}

	return -1;
}
