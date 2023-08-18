#pragma once

#include <Windows.h>
#include <cstdint>

namespace state {
    extern HANDLE CurrentProcess;
    extern uint16_t pid;

    extern bool isRunning;
    extern bool popup;
    extern bool disassembled;
    extern bool mapped_strings;
}