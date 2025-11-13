@echo off

echo ========================================
echo    Max-CMOS Build
echo ========================================

set BUILD_PATH=build\
set PROJECTS_PATH=%BUILD_PATH%\source\projects

if exist %BUILD_PATH% (
    echo Build directory exists
) else (
    echo Creating build directory...
    mkdir %BUILD_PATH%
)

cd %BUILD_PATH%

echo Configuring project with CMake...
cmake -G "Visual Studio 17 2022" .. -DCMAKE_POLICY_VERSION_MINIMUM=3.5

if %errorlevel% neq 0 (
    echo CMake configuration failed!
    exit /b %errorlevel%
)

echo Building project...

if "%1"=="--clean-first" (
    echo Cleaning build directory...
    cmake --build . --config Release --clean-first
) else (
    cmake --build . --config Release
)

if %errorlevel% neq 0 (
    echo Build failed!
    exit /b %errorlevel%
)

echo Running tests...
ctest -C Release --output-on-failure

if %errorlevel% neq 0 (
    echo Some tests failed!
    exit /b %errorlevel%
)

echo ========================================
echo    Build and tests completed successfully!
echo ========================================
