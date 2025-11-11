#include <type_traits>
#include "../Exceptions/Exceptions.hpp"
#include "Counter.hpp"

int Counter::reset() {
    // Reset the counter.
    this->counter = this->first_step;
    return this->counter;
}

int Counter::forward() {
    // Step forward by one.
    if (this->counter < (this->max_value - 1)) {
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
        this->counter = (this->max_value - 1);
    }

    return this->counter;
}

bool Counter::direction(){
    this->dir = !this->dir;
    return this->dir;
}

int Counter::set(int val) {
    // Set the value of the counter.
    if (0 <= val && val < this->max_value){
        this->counter = val;
    }

    return this->counter;
}

int Counter::preset(){
    // Activate the preset value.
    this->counter = this->preset_value;
    return this->counter;
}

int Counter::step(){
    if (this->dir){
        return this->forward();
    }
    
    return this->back();
}
