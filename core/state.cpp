#include "state.h"

HANDLE state::CurrentProcess = NULL;
uint16_t state::pid = 0;
bool state::isRunning = true;
bool state::popup = true;
bool state::disassembled = false;
bool state::mapped_strings = false;