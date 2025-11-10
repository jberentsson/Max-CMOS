#include <type_traits>
#include "Counter.hpp"

int Counter::reset() {
    // Reset the counter.
    this->counter = this->first_step;
    return this->counter;
}

int Counter::forward() {
    // Step forward by one.
    if (this->counter < (this->steps - 1)) {
        this->counter++;
    } else {
        this->counter = this->first_step;
    }

    return this->counter;
}

int Counter::back() {
    // Step back by one.
    if (this->counter > 0) {
        this->counter--;
    } else {
        this->counter = (this->steps - 1);
    }

    return this->counter;
}

int Counter::set(int val) {
    // Set the value of the counter.
    this->counter = val;
    return this->counter;
}

int Counter::preset(){
    // Activate the preset value.
    this->counter = this->preset_value;
    return this->counter;
}
