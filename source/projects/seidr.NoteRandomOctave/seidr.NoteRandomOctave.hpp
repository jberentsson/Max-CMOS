#pragma once
#include "Keyboard/Keyboard.hpp"
#include <c74_min.h>

// Forward declaration
class Keyboard;

class NoteRandomOctave : public c74::min::object<NoteRandomOctave> {
private:
	Keyboard keyboard = Keyboard();

public:
	explicit NoteRandomOctave(const c74::min::atoms& args = {});
	~NoteRandomOctave();

	void processNoteMessage(int note, int velocity);
	void clearNoteMessage(int note);
	void clearAllNotesMessage();
	void setRangeMessage(int low, int high);
	void printActiveNotes();

	// Inlets and outlets
	c74::min::inlet<>  input_0 {this, "(int) note, (int) velocity"};
	c74::min::outlet<> output_0 {this, "(int) pitch"};
	c74::min::outlet<> output_1 {this, "(int) velocity"};

	// Messages - declare them here but define the handlers separately
	c74::min::message<> anything;
	c74::min::message<> clear;
	c74::min::message<> clearall;
	c74::min::message<> range;
	c74::min::message<> debug;
};