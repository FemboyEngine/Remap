#pragma once

#include <Windows.h>
#include <cstdint>
#include <vector>
#include <string>

namespace state {
    extern HANDLE CurrentProcess;
    extern LPCVOID BaseAddress;
    extern uint16_t pid;

    extern std::vector<uint8_t> memory;
    extern std::vector<std::string> modules;
    extern std::vector<int> aob;

    extern bool isRunning;
    extern bool popup;
    extern bool disassembled;
    extern bool mapped_strings;
}