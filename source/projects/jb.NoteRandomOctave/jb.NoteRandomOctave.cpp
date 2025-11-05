/// @file       jb.NoteRandomOctave.cpp
///	@ingroup 	jb
///	@copyright	Copyright 2025 - Jóhann Berentsson. All rights reserved.
///	@license	Use of this source code is governed by the MIT License found in the License.md file.

#include "jb.NoteRandomOctave.hpp"
#include "Keyboard.cpp"

using namespace c74::min;

NoteRandomOctave::NoteRandomOctave(const atoms& args) {
    keyboard = new Keyboard();
}

NoteRandomOctave::~NoteRandomOctave() {
    delete keyboard;
}

MIN_EXTERNAL(NoteRandomOctave);
