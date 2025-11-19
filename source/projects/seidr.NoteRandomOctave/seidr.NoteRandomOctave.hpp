#pragma once
#include "Keyboard/Keyboard.hpp"

#include <c74_min.h>

// Forward declaration
class Keyboard;

class NoteRandomOctaveMax : public c74::min::object<NoteRandomOctaveMax> {
  private:
    Keyboard keyboard_;

  public:
    explicit NoteRandomOctaveMax(const c74::min::atoms &args = {});
    ~NoteRandomOctaveMax();

    void processNoteMessage(int note, int velocity);
    void clearNoteMessage(int note);
    void clearAllNotesMessage();
    void setRangeMessage(int low, int high);
    void printActiveNotes();

    // Inlets and outlets
    c74::min::inlet<> input0{this, "(int) note, (int) velocity"};
    c74::min::outlet<> output0{this, "(int) pitch"};
    c74::min::outlet<> output1{this, "(int) velocity"};

    // Messages with handlers defined inline
    c74::min::message<> anything{this, "anything", "Process note messages",
                                 MIN_FUNCTION{if (args.size() >= 2){int note = args[0];
    int velocity = args[1];
    processNoteMessage(note, velocity);
} return {};
}
}
;

c74::min::message<> clear{this, "clear", "Clear specific note",
                          MIN_FUNCTION{if (args.size() >= 1){int note = args[0];
clearNoteMessage(note);
}
return {};
}
}
;

c74::min::message<> clearall{this, "clearall", "Clear all notes",
                             MIN_FUNCTION{clearAllNotesMessage();
return {};
}
}
;

c74::min::message<> range{this, "range", "Set range",
                          MIN_FUNCTION{if (args.size() >= 2){int low = args[0];
int high = args[1];
setRangeMessage(low, high);
}
return {};
}
}
;

c74::min::message<> debug{this, "debug", "Print debug info", MIN_FUNCTION{printActiveNotes();
return {};
}
}
;
}
;