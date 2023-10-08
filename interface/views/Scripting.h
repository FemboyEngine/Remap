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

        uintptr_t address = lua_tointeger(L, 1);

        int pid = lua_tointeger(L, 2);
        int size = lua_tointeger(L, 3);

        HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

        if (hProcess == INVALID_HANDLE_VALUE) {
            lua_pushinteger(L, 0);
            return 1;
        }

        void* buffer = malloc(size);
        SIZE_T bytesRead;

        ReadProcessMemory(hProcess, (LPCVOID)address, buffer, size, &bytesRead);

        lua_pushinteger(L, (uintptr_t)buffer);
        lua_pushinteger(L, bytesRead);

        return 2;
    });
    lua_setglobal(L, "rpm");

    lua_pushcfunction(L, [](lua_State* L) -> int {
		uintptr_t address = lua_tointeger(L, 1);
		int pid = lua_tointeger(L, 2);
		int size = lua_tointeger(L, 3);
		void* buffer = (void*)lua_tointeger(L, 4);
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
        if (hProcess == INVALID_HANDLE_VALUE) {
			lua_pushinteger(L, 0);
			return 1;
		}
		SIZE_T bytesWritten;
		WriteProcessMemory(hProcess, (LPVOID)address, buffer, size, &bytesWritten);
		lua_pushinteger(L, bytesWritten);
		return 1;
	});
    lua_setglobal(L, "wpm");

    lua_pushcfunction(L, [](lua_State* L) -> int {
        int number = lua_tointeger(L, 1);
        std::stringstream ss;
        ss << "0x" << std::hex << number;
        lua_pushstring(L, ss.str().c_str());
        return 1;
        });
    lua_setglobal(L, "ToHex");

    ImGui::InputText("##input", l_buffer, IM_ARRAYSIZE(l_buffer));
    ImGui::SameLine();
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