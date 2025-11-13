#pragma once

#define MAX_VALUE 10

class Counter {
    protected:
        bool is_enabled = false;
        bool dir = true;
        unsigned int first_step = 0;
        unsigned int counter = 0;
        unsigned int max_value = MAX_VALUE;
        unsigned int preset_value = 0;

    public:
        Counter(){};
        Counter(int n){ this->max_value = n; };

        virtual bool direction();  // Toggles and returns new direction
        virtual bool get_direction() { return this->dir; }  // Just returns current direction
        virtual int value(); 
        virtual int set(int val);
        virtual int forward();
        virtual int preset();
        virtual int reset();
        virtual int back();
        virtual int step();
        virtual bool toggle_direction();
        
        virtual ~Counter() = default;
};