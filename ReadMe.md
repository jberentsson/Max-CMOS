# CMOS 4000
This package was created using the Min-DevKit for Max, an API and supporting tools for writing externals in modern C++.

## Build

To create build files run:
```bash
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release
```

Build the project with.
```bash
cmake --build . --config Release
```

Build a specific target.
````bash
cmake --build . --config Release --target catch2_test_Note_test
```W

## Notes
There was a conflict between catch2 and min-api so the test framework had to be switched to doctest.
