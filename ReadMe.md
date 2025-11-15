# Max Modules
This package was created using the Min-DevKit for Max, an API and supporting tools for writing externals in modern C++.

## Build
```bash
# Setup CMake.
cmake -B build

# Build the project.
cmake --build build --config Release

# Or if you want fresh build.
cmake --build build --config Release --clean-first

# Or if you want fresh build.
cmake --build build --config Release --clean-first --target <TARGET_NAME>
```

## Available Targets
### Projects:
- jb.BinaryCounter
- jb.BinaryCounter_test
- jb.NCounter
- jb.NCounter_test
- jb.RandomNoteOctave
- jb.RandomNoteOctave_tess
- jb.ShiftRegister
- jb.ShiftRegister_test

### Libraries:
- BinaryCounter
- BinaryCounter_test
- Counter
- Counter_test
- Keyboard
- Keyboard_test
- RandomNoteOctave
- RandomNoteOctave_test
- ShiftRegister
- ShiftRegister_test

