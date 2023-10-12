#include <Windows.h>
#include <cstdint>
#include <vector>
#include <string>

namespace state {
    HANDLE CurrentProcess = NULL;
    LPCVOID BaseAddress = nullptr;
    uint16_t pid = 0;

    std::vector<uint8_t> memory;
    std::vector<std::string> modules;

    bool disassembled = false;
    bool mapped_strings = false;
}