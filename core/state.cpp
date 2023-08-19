#include "state.h"

HANDLE state::CurrentProcess = NULL;
LPCVOID state::BaseAddress = nullptr;
uint16_t state::pid = 0;

std::vector<uint8_t> state::memory;
std::vector<std::string> state::modules;

bool state::isRunning = true;
bool state::popup = true;
bool state::disassembled = false;
bool state::mapped_strings = false;