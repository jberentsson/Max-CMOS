#include <type_traits>
#include "Counter.hpp"

Counter::Counter() {

}

Counter::Counter(int n){
    this->steps = n;
}

int Counter::reset() {
    this->counter = 0;
    return this->counter;
}

int Counter::step() {
    if (this->dir == 0) {
        this->counter++;
    } else {
        this->counter--;
    }

    if (this->counter == this->steps) {
        this->counter = 0;
    }

    return this->counter;
}

int Counter::current() {
    return this->counter;
}

int Counter::direction() {
    return this->dir;
}

int Counter::back(){
    this->dir = 1;
    step();
    return this->counter;
}

int Counter::forward(){
    this->dir = 0;
    step();
    return this->counter;
}

int Counter::set(int val){
    this->counter = val;
    return this->counter;
}

