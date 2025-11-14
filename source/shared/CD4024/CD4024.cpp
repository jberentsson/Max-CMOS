#include <cmath>
#include "CD4024.hpp"

CD4024::CD4024(int n) {
    this->set_max_value(n);
}

int CD4024::set_max_value(int n){
    this->max_value = std::pow(2, n);
    return this->max_value;
}

int CD4024::get_max_value(){
    return this->max_value;
}
