# seidr.RandomOctave

## Description
This is a module that takes a note and randomizes the octave. To do this properly it requires the velocity to detect when the original note has been deactivated.

### Inputs:
1. (list) Note Velocity

### Outputs:
1. (list) Note Velocity

### Messages:
- [clear i] : clear a note from 0 127
- [clearall] : clear all notes
- [i i] : [note velocity]
- [range h l] : sets the min and max note ouput value
