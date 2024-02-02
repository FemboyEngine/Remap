/*
* Code for the lua scripting view
* - views/Scripting.h
*/

extern "C" {
    #include <lua.h>
    #include <lauxlib.h>
    #include <lualib.h>
}

#define LUA_ERR(L, msg) \
 do { \
   DWORD error = GetLastError(); \
   std::string message = std::string(msg) + ". Error code: " + std::to_string(error); \
   lua_pushstring(L, message.c_str()); \
   return lua_error(L); \
 } while (0)

/**
* Temporary, move after using header-source
* Project structure
*/
static std::string OutputBuffer;

class ScriptingView : public View {
public:
    ScriptingView() : View("Scripting") {}

protected:
    /**
     * Render the Lua scripting window.
     * Provides a Lua scripting environment for executing Lua scripts and displays the script output
     */
    void Content() override {
        lua_State* L = luaL_newstate();
        luaL_openlibs(L);

        RegisterClosures(L);

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
    }

private:
    char l_buffer[1024];

    static int l_print(lua_State* L) {
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
                ss << '\t';
            }
            ss << s;
            lua_pop(L, 1);
        }
        OutputBuffer += ss.str();
        OutputBuffer += '\n';
        return 0;
    }

    static int GetProcessName(lua_State* L) {
        int pid = lua_tointeger(L, 1);
        lua_pushstring(L, remap::GetProcessName(pid).data());
        return 1;
    }

    static int GetProcessesNames(lua_State* L) {
        auto process_names = remap::GetProcessesNames();
        lua_newtable(L);
        for (size_t i = 0; i < process_names.size(); ++i) {
            lua_pushinteger(L, i + 1);
            lua_pushstring(L, process_names[i].data());
            lua_settable(L, -3);
        }
        return 1;
    }

    static int GetPIDByName(lua_State* L) {
        const char* process_name = lua_tostring(L, 1);
        lua_pushinteger(L, remap::GetProcessIdByName(process_name));
        return 1;
    }

    static int GetBaseAddress(lua_State* L) {
        int pid = lua_tointeger(L, 1);
        lua_pushinteger(L, remap::GetProcessBaseAddress(pid));
        return 1;
    }
    
    static int GetModules(lua_State* L) {
        int pid = lua_tointeger(L, 1);
        auto modules = remap::GetLoadedModules(pid);
        lua_newtable(L);
        for (size_t i = 0; i < modules.size(); ++i) {
            lua_pushinteger(L, i + 1);
            lua_pushstring(L, modules[i].data());
            lua_settable(L, -3);
        }
        return 1;
    }

    static int Read(lua_State* L) {
        DWORD pid = lua_tointeger(L, 1);
        uintptr_t address = lua_tointeger(L, 2);
        size_t size = lua_tointeger(L, 3);
        std::unique_ptr<char[]> buffer(new char[size]);
        SIZE_T bytesRead;
        HANDLE handle = OpenProcess(PROCESS_VM_READ, FALSE, pid);
        if (handle != NULL) {
            if (ReadProcessMemory(handle, (LPCVOID)address, buffer.get(), size, &bytesRead)) {
                lua_pushlstring(L, buffer.get(), bytesRead);
            }
            else {
                LUA_ERR(L, "Failed to read memory");
            }
            CloseHandle(handle);
        }
        else {
            LUA_ERR(L, "Failed to open process");
        }
        return 1;
    }

    static int Write(lua_State* L) {
        DWORD pid = lua_tointeger(L, 1);
        uintptr_t address = lua_tointeger(L, 2);
        const char* data = lua_tostring(L, 3);
        size_t size = strlen(data);
        std::unique_ptr<char[]> buffer(new char[size]);
        memcpy(buffer.get(), data, size);
        SIZE_T bytesWritten;
        HANDLE handle = OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, pid);
        if (handle != NULL) {
            if (WriteProcessMemory(handle, (LPVOID)address, buffer.get(), size, &bytesWritten)) {
                lua_pushboolean(L, true);
            }
            else {
                LUA_ERR(L, "Failed to write memory");
            }
            CloseHandle(handle);
        }
        else {
            LUA_ERR(L, "Failed to open process");
        }
        return 1;
    }

    static int ToHex(lua_State* L) {
        int number = lua_tointeger(L, 1);
        std::string hex = "0x" + std::format("{:x}", number);
        lua_pushstring(L, hex.data());
        return 1;
    }

    void RegisterClosures(lua_State* L) {
        std::unordered_map<std::string, lua_CFunction> functions = {
            {"print", l_print},
            {"procname", GetProcessName},
            {"procnames", GetProcessesNames},
            {"idbyname", GetPIDByName},
            {"baseaddr", GetBaseAddress},
            {"getmods", GetModules},
            {"read", Read},
            {"write", Write}
        };

        for (const auto& pair : functions) {
            lua_pushcfunction(L, pair.second);
            lua_setglobal(L, pair.first.c_str());
        }
    }
};