#pragma once

#include <cstdlib>
#include <ctime>
#include <random>
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>

#define NOTE_CAPACITY 8
#define OCTAVE 12

class ActiveNote {
    private:
        int notePitch;

    public:
        ActiveNote(int n){
            this->notePitch = n;
        }

        int pitch(){
            return this->notePitch;
        }
};

class Note {
private:
    int notePitch;
    int count = 0;
    int minOctave = 0;
    int maxOctave = NOTE_CAPACITY;
    int rangeLow;
    int rangeHigh;
    std::vector<std::unique_ptr<ActiveNote>> activeNotes;

public:
    Note(int p, int* low, int* high);
    
    std::vector<ActiveNote*> add(int n);
    std::vector<ActiveNote*> removeAll();
    std::vector<std::unique_ptr<ActiveNote>>& active();
    std::vector<ActiveNote*> getActiveNotes();

    int randomizeNote();
    int clearNotes();
    int maxCapacity();
    int minCapacity();
};
