/*
* Remap
* A memory editor, written in C++ and ImGui.
*
* This file is part of Remap.
* - core/memory.hpp
*
* Code for memory editing.
*/

#pragma once

#include <vector>
#include <string>
#include <Windows.h>
#include <TlHelp32.h>
#include <algorithm>
#include <Psapi.h>

const int MAX_SEARCH_BUFFER_SIZE = 256;
const int MAX_BUFFER_SIZE = 1024 * 1024 * 10;

std::string GetProcessName(int pid) {
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
	if (hProcess == nullptr) {
		return "";
	}
	char szProcessName[MAX_PATH];
	if (GetModuleBaseNameA(hProcess, nullptr, szProcessName, sizeof(szProcessName)) > 0) {
		CloseHandle(hProcess);
		return szProcessName;
	}
	CloseHandle(hProcess);
	return "";
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
	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32;
	int pid = 0;
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE) {
		return pid;
	}
	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hProcessSnap, &pe32)) {
		CloseHandle(hProcessSnap);
		return pid;
	}
	do {
		char szExeFile[MAX_PATH];
		WideCharToMultiByte(CP_ACP, 0, pe32.szExeFile, -1, szExeFile, MAX_PATH, NULL, NULL);

		if (strcmp(szExeFile, process_name) == 0) {
			pid = pe32.th32ProcessID;
			break;
		}
	} while (Process32Next(hProcessSnap, &pe32));
	CloseHandle(hProcessSnap);
	return pid;
}

uintptr_t GetProcessBaseAddress(int pid) {
	uintptr_t base_address = 0;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	HMODULE hMods[1024];
	DWORD cbNeeded;

	if (hProcess == nullptr) {
		return base_address;
	}

	if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded)) {
		for (unsigned int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++) {
			TCHAR szModName[MAX_PATH];
			if (GetModuleFileNameEx(hProcess, hMods[i], szModName, sizeof(szModName) / sizeof(TCHAR))) {
				base_address = (uintptr_t)hMods[i];
				break;
			}
		}
	}

	CloseHandle(hProcess);

	return base_address;
}

std::vector<std::string> GetLoadedModules(int pid) {
	std::vector<std::string> modules;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	HMODULE hMods[1024];
	DWORD cbNeeded;

	if (hProcess == nullptr) {
		return modules;
	}
	if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded)) {
		for (unsigned int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++) {
			TCHAR szModName[MAX_PATH];
			if (GetModuleFileNameEx(hProcess, hMods[i], szModName, sizeof(szModName) / sizeof(TCHAR))) {
				char szModNameA[MAX_PATH];
				WideCharToMultiByte(CP_ACP, 0, szModName, -1, szModNameA, MAX_PATH, NULL, NULL);

				// remove path from module name
				std::string szModNameStr = szModNameA;
				std::string::size_type pos = szModNameStr.find_last_of("\\/");
				if (pos != std::string::npos) {
					szModNameStr.erase(0, pos + 1);
				}

				modules.push_back(szModNameStr);
			}
		}
	}
	CloseHandle(hProcess);
	return modules;
}