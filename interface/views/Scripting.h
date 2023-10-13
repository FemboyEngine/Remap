/*
* Remap
* A memory editor, written in C++ and ImGui.
*
* This file is part of Remap.
* - views/Scripting.h
*
* Code for the lua scripting view.
*/

extern "C" {
    #include <lua.h>
    #include <lauxlib.h>
    #include <lualib.h>
}

static std::string OutputBuffer;
char l_buffer[1024];

int l_print(lua_State* L) {
    std::stringstream ss;
    int n = lua_gettop(L);
    lua_getglobal(L, "tostring");
    for (int i = 1; i <= n; i++) {
        lua_pushvalue(L, -1);
        lua_pushvalue(L, i);
        lua_call(L, 1, 1);
        const char* s = lua_tostring(L, -1);
        if (s == NULL) {
            return luaL_error(L, "'tostring' must return a string to 'print'");
        }
        if (i > 1) {
            ss << "\t";
        }
        ss << s;
        lua_pop(L, 1);
    }
    OutputBuffer += ss.str() + "\n";
    return 0;
}

void ui::views::Scripting() {

    if (!states::running["Scripting"])
		return;

    ImGui::Begin(
        "Scripting",
        &states::running["Scripting"],
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_HorizontalScrollbar |
        ImGuiWindowFlags_NoSavedSettings
    );

    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    lua_pushcfunction(L, l_print);
    lua_setglobal(L, "print");

    // push memory functions
    lua_pushcfunction(L, [](lua_State* L) -> int {
        int pid = lua_tointeger(L, 1);
        lua_pushstring(L, remap::GetProcessName(pid).c_str());
        return 1;
        });
    lua_setglobal(L, "GetProcessName");

    lua_pushcfunction(L, [](lua_State* L) -> int {
        std::vector<std::string> process_names = remap::GetProcessesNames();
        lua_newtable(L);
        for (int i = 0; i < process_names.size(); i++) {
            lua_pushinteger(L, i + 1);
            lua_pushstring(L, process_names[i].c_str());
            lua_settable(L, -3);
        }
        return 1;
        });
    lua_setglobal(L, "GetProcessesNames");

    lua_pushcfunction(L, [](lua_State* L) -> int {
        const char* process_name = lua_tostring(L, 1);
        lua_pushinteger(L, remap::GetProcessIdByName(process_name));
        return 1;
        });
    lua_setglobal(L, "GetProcessIdByName");

    lua_pushcfunction(L, [](lua_State* L) -> int {
        int pid = lua_tointeger(L, 1);
        lua_pushinteger(L, remap::GetProcessBaseAddress(pid));
        return 1;
        });
    lua_setglobal(L, "GetProcessBaseAddress");

    lua_pushcfunction(L, [](lua_State* L) -> int {
        int pid = lua_tointeger(L, 1);
        std::vector<std::string> modules = remap::GetLoadedModules(pid);
        lua_newtable(L);
        for (int i = 0; i < modules.size(); i++) {
            lua_pushinteger(L, i + 1);
            lua_pushstring(L, modules[i].c_str());
            lua_settable(L, -3);
        }
        return 1;
        });
    lua_setglobal(L, "GetLoadedModules");

    lua_pushcfunction(L, [](lua_State* L) -> int {
        DWORD pid = lua_tointeger(L, 1);
        uintptr_t address = lua_tointeger(L, 2);
        size_t size = lua_tointeger(L, 3);
        char* buffer = new char[size];
        SIZE_T bytesRead;
        HANDLE processHandle = OpenProcess(PROCESS_VM_READ, FALSE, pid);
        if (processHandle != NULL) {
            if (ReadProcessMemory(processHandle, (LPCVOID)address, buffer, size, &bytesRead)) {
                lua_pushlstring(L, buffer, bytesRead);
            }
            else {
                DWORD error = GetLastError();
                std::stringstream ss;
                ss << "Failed to read memory. Error code: " << error;
                lua_pushstring(L, ss.str().c_str());
            }
            CloseHandle(processHandle);
        }
        else {
            DWORD error = GetLastError();
            std::stringstream ss;
            ss << "Failed to open process. Error code: " << error;
            lua_pushstring(L, ss.str().c_str());
        }
        delete[] buffer;
        return 1;
    });
    lua_setglobal(L, "read");

    lua_pushcfunction(L, [](lua_State* L) -> int {
        DWORD pid = lua_tointeger(L, 1);
        uintptr_t address = lua_tointeger(L, 2);
        size_t size = strlen(lua_tostring(L, 3));
        char* buffer = new char[size];
        memcpy(buffer, lua_tostring(L, 3), size);
        SIZE_T bytesWritten;
        HANDLE processHandle = OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, pid);
        if (processHandle != NULL) {
            if (WriteProcessMemory(processHandle, (LPVOID)address, buffer, size, &bytesWritten)) {
                lua_pushboolean(L, true);
            }
            else {
                DWORD error = GetLastError();
                std::stringstream ss;
                ss << "Failed to write memory. Error code: " << error;
                lua_pushstring(L, ss.str().c_str());
            }
            CloseHandle(processHandle);
        }
        else {
            DWORD error = GetLastError();
            std::stringstream ss;
            ss << "Failed to open process. Error code: " << error;
            lua_pushstring(L, ss.str().c_str());
        }
        delete[] buffer;
        return 1;
	});
    lua_setglobal(L, "write");

    lua_pushcfunction(L, [](lua_State* L) -> int {
        int number = lua_tointeger(L, 1);
        std::stringstream ss;
        ss << "0x" << std::hex << number;
        lua_pushstring(L, ss.str().c_str());
        return 1;
        });
    lua_setglobal(L, "ToHex");

    ImGui::InputTextMultiline("##input", l_buffer, IM_ARRAYSIZE(l_buffer),
        ImVec2(ImGui::GetContentRegionAvail().x, 0));

    if (ImGui::Button("Load")) {
        int status = luaL_dostring(L, l_buffer);

        if (status != LUA_OK) {
            OutputBuffer += lua_tostring(L, -1);
            OutputBuffer += "\n";
            lua_pop(L, 1);
        }
    }

    ImGui::SameLine();
    if (ImGui::Button("Clear")) {
        OutputBuffer.clear();
    }

    ImGui::BeginChild("Output", ImVec2(0, 0), true);
    ImGui::TextUnformatted(OutputBuffer.c_str());
    ImGui::EndChild();

    ImGui::End();
}