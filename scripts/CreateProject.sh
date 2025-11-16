#!/usr/bin/env bash
#
# ========================= DESCRIPTION =========================
# This scripts creates all of the files needed 
# to start programming a new MAX/MSP project.
#
# ========================= FILES ===============================
# The files and directories that are created are.
# - source/Projects/NewProjectName/
# - source/Projects/NewProjectName/CMakeLists.txt
# - source/Projects/NewProjectName/NewProjectName.cpp
# - source/Projects/NewProjectName/NewProjectName.hpp
# - source/Projects/NewProjectName/NewProjectName_test.hpp
#
# ========================= USAGE ===============================
# ./scripts/CrateProject.sh
#

create_folder(){
    mkdir -p "$NEW_PROJECT_DIRECTORY"
    echo "---- Folder created at: $NEW_PROJECT_DIRECTORY"
}

create_file(){
    touch "$NEW_PROJECT_PATH""$1"
    echo "---- File created at: $NEW_PROJECT_PATH"
}

if [ $# -eq 0 ]; then
    echo "No project name specified!"
else
    NEW_PROJECT_NAME=$1
    NEW_PROJECT_DIRECTORY="source/projects/$NEW_PROJECT_NAME"
    NEW_PROJECT_PATH="$NEW_PROJECT_DIRECTORY/"

    if [ ! -d $NEW_PROJECT_DIRECTORY ]; then
        echo "Creating new project files for $NEW_PROJECT_NAME"

        # Create folder.
        echo "-- Creating folder..."
        create_folder

        # Create header.
        echo "-- Creating CMake file..."
        create_file "CMakeLists.txt"

        # Create header.
        echo "-- Creating header file..."
        create_file "$NEW_PROJECT_NAME"".hpp"

        # Create main file.
        echo "-- Creating header file..."
        create_file "$NEW_PROJECT_NAME"".cpp"

        # Create test file.
        echo "-- Creating test file..."
        create_file "$NEW_PROJECT_NAME""_test.cpp"
    else
        echo "Project folder already exists!"
    fi
fi
