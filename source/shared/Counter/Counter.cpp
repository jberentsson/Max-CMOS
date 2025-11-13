#include <type_traits>
#include "../Exceptions/Exceptions.hpp"
#include "Counter.hpp"

int Counter::reset() {
    this->counter = this->first_step;
    return this->counter;
}

int Counter::forward() {
    // Improved forward with proper wrapping
    this->counter++;
    if (this->counter >= this->max_value) {
        this->counter = this->first_step;
    }
    return this->counter;
}

int Counter::back() {
    // Improved backward with proper wrapping  
    if (this->counter == this->first_step) {
        this->counter = this->max_value - 1;
    } else {
        this->counter--;
    }
    return this->counter;
}

bool Counter::direction(){
    return this->dir;
}

// Add function to toggle direction if needed
bool Counter::toggle_direction(){
    this->dir = !this->dir;
    return this->dir;
}

int Counter::set(int val) {
    if (0 <= val && val < this->max_value){
        this->counter = val;
    }
    return this->counter;
}

int Counter::preset(){
    this->counter = this->preset_value;
    return this->counter;
}

int Counter::step(){
    if (this->dir){
        return this->forward();
    }
    return this->back();
}

int Counter::value() { 
    return this->counter; 
}
