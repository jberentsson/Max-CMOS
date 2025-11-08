@echo off

echo ========================================
echo    Max-CMOS Build
echo ========================================

set BUILD_PATH = "build"
set PROJECTS_PATH = "%BUILD_PATH%/source/projects"

@REM if exist  (
    @REM rem file exists
@REM ) else (
    @REM rem file doesn't exist
@REM )

if "%1"=="--clean" (
    rmdir /S /Q "build/source/projects"
)

cd build

cmake -G "Visual Studio 17 2022" ..

cmake --build . --config Release
