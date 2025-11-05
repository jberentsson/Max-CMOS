#pragma once
#include <vector>
#include <memory>
#include "Note.hpp"

#define RANGE_MIN 0
#define RANGE_MAX 127
#define NOTES 128

class Keyboard {
    private:
        std::vector<std::unique_ptr<Note>> notes; 
        
    protected:
        int rangeLow = RANGE_MIN;
        int rangeHigh = RANGE_MAX;

    public:
        Keyboard();
        std::vector<ActiveNote*> note(int note, int velocity);
        int clearNotes(int n);
        int setRandomRange(int l, int h);
};
