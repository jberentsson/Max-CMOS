#include "CD4017.hpp"

CD4017::CD4017(int n) {
    this->steps = n;
}

int CD4017::step() {
    // Step forward by one.
    return this->forward();
}
