/*
* Code for memory helpers.
* - core/memory.h
*/

#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <vector>
#include <string>
#include <Windows.h>
#include <TlHelp32.h>
#include <algorithm>
#include <Psapi.h>
#include <sstream>

const int kMaxSearchBufferSize = 256;
const int kMaxBufferSize = 1024 * 1024 * 10;

namespace remap {

    std::string GetProcessName(uint32_t pid) {
        HANDLE h_process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
        if (h_process == nullptr) {
            return std::string();
        }
        char sz_process_name[MAX_PATH];
        if (GetModuleBaseNameA(h_process, nullptr, sz_process_name, sizeof(sz_process_name)) > 0) {
            CloseHandle(h_process);
            return sz_process_name;
        }
        CloseHandle(h_process);
        return std::string(); // ""
    }

    std::vector<std::string> GetProcessesNames() {
        DWORD processes[1024], cb_needed;
        std::vector<std::string> process_names;
        if (!EnumProcesses(processes, sizeof(processes), &cb_needed)) {
            return process_names;
        }
        DWORD num = cb_needed / sizeof(DWORD);
        for (DWORD i = 0; i < num; i++) {
            std::string process_name = GetProcessName(processes[i]);
            if (!process_name.empty()) {
                process_names.push_back(process_name);
            }
        }
        return process_names;
    }

    int GetProcessIdByName(const char* process_name) {
        HANDLE h_process_snap;
        PROCESSENTRY32 pe32;
        int pid = 0;
        h_process_snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (h_process_snap == INVALID_HANDLE_VALUE) {
            return pid;
        }
        pe32.dwSize = sizeof(PROCESSENTRY32);
        if (!Process32First(h_process_snap, &pe32)) {
            CloseHandle(h_process_snap);
            return pid;
        }
        do {
            char sz_exe_file[MAX_PATH];
            WideCharToMultiByte(CP_ACP, 0, pe32.szExeFile, -1, sz_exe_file, MAX_PATH, NULL, NULL);

            if (strcmp(sz_exe_file, process_name) == 0) {
                pid = pe32.th32ProcessID;
                break;
            }
        } while (Process32Next(h_process_snap, &pe32));
        CloseHandle(h_process_snap);
        return pid;
    }

    uintptr_t GetProcessBaseAddress(uint32_t pid) {
        uintptr_t base_address = 0;
        HANDLE h_process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
        HMODULE h_mods[1024];
        DWORD cb_needed;

        if (h_process == nullptr) {
            return base_address;
        }

        if (EnumProcessModules(h_process, h_mods, sizeof(h_mods), &cb_needed)) {
            for (unsigned int i = 0; i < (cb_needed / sizeof(HMODULE)); i++) {
                TCHAR sz_mod_name[MAX_PATH];
                if (GetModuleFileNameEx(h_process, h_mods[i], sz_mod_name, sizeof(sz_mod_name) / sizeof(TCHAR))) {
                    base_address = (uintptr_t)h_mods[i];
                    break;
                }
            }
        }

        CloseHandle(h_process);

        return base_address;
    }

    std::vector<std::string> GetLoadedModules(uint32_t pid) {
        std::vector<std::string> modules;
        HANDLE h_process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
        HMODULE h_mods[1024];
        DWORD cb_needed;

        if (h_process == nullptr) {
            return modules;
        }
        if (EnumProcessModules(h_process, h_mods, sizeof(h_mods), &cb_needed)) {
            for (unsigned int i = 0; i < (cb_needed / sizeof(HMODULE)); i++) {
                TCHAR sz_mod_name[MAX_PATH];
                if (GetModuleFileNameEx(h_process, h_mods[i], sz_mod_name, sizeof(sz_mod_name) / sizeof(TCHAR))) {
                    char sz_mod_name_a[MAX_PATH];
                    WideCharToMultiByte(CP_ACP, 0, sz_mod_name, -1, sz_mod_name_a, MAX_PATH, NULL, NULL);

                    // remove path from module name
                    std::string sz_mod_name_str = sz_mod_name_a;
                    std::string::size_type pos = sz_mod_name_str.find_last_of("\\/");
                    if (pos != std::string::npos) {
                        sz_mod_name_str.erase(0, pos + 1);
                    }

                    modules.push_back(sz_mod_name_str);
                }
            }
        }
        CloseHandle(h_process);
        return modules;
    }

} // namespace remap

#endif // MEMORY_HPP