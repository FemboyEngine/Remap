/*
* Code for global variables.
* - core/state.h
*/

#include <Windows.h>
#include <cstdint>
#include <vector>
#include <string>

namespace state {
    HANDLE current_process = NULL;
    LPCVOID base_address = nullptr;
    uint32_t pid = 0;

    std::vector<uint8_t> memory;
    std::vector<std::string> modules;

    bool disassembled = false;
    bool mapped_strings = false;
}