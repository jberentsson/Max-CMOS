# Max Modules
## Description
This package was created using the Min-DevKit for Max, an API and supporting tools for writing externals in modern C++.

## Build Status
![CI/CD Main](https://github.com/jberentsson/seidr/actions/workflows/ci-cd.yml/badge.svg?branch=main)
![CI/CD Develop](https://github.com/jberentsson/seidr/actions/workflows/ci-cd.yml/badge.svg?branch=develop)

| **Main** | ![Main Branch](https://github.com/jberentsson/seidr/actions/workflows/release.yml/badge.svg?branch=main) |
| **Develop** | ![Develop Branch](https://github.com/jberentsson/seidr/actions/workflows/release.yml/badge.svg?branch=develop) |

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
- seidr.BinaryCounter
- seidr.BinaryCounter_test
- seidr.NCounter
- seidr.NCounter_test
- seidr.RandomNoteOctave
- seidr.RandomNoteOctave_tess
- seidr.ShiftRegister
- seidr.ShiftRegister_test

### Libraries:
- BinaryCounter
- BinaryCounter_test
- Counter
- Counter_test
- RandomOctave
- RandomOctave_test
- RandomNoteOctave
- RandomNoteOctave_test
- ShiftRegister
- ShiftRegister_test


