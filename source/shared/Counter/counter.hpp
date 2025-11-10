#pragma once

#define DEFAULT_STEP_LENGTH 10;

class Counter {
    protected:
		bool is_enabled = false;
        unsigned int first_step = 0;
        unsigned int counter = 0;
        unsigned int steps = DEFAULT_STEP_LENGTH;
        unsigned int dir = 0;
        unsigned int preset_value = 0;
        int step(){ return 0; }

    public:
        Counter(){};
        Counter(int n){ this->steps = n; };

        virtual int direction(){ return this->dir; }
        virtual int current() { return this->counter; }
        virtual int set(int val);
        virtual int forward();
        virtual int preset();
        virtual int reset();
        virtual int back();
};
