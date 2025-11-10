#pragma once

#define DEFAULT_STEP_LENGTH 10;

class Counter {
    protected:
        unsigned int counter = 0;
        unsigned int steps = DEFAULT_STEP_LENGTH;
        unsigned int dir = 0;

    public:
        Counter();
        Counter(int n);
        int current();
        int reset();
        int step();
        int direction();
        int forward();
        int back();
        int set(int val);
};
