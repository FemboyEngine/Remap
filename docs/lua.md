## Lua Scripting Support

Remap provides a Lua scripting interface that allows you to interact with processes and their memory from Lua. This document describes the available Remap API Lua functions and how to use them.

### Getting Started

1. Make sure that the "Scripting" view is enabled and open in the utility.

2. You can enter your Lua scripts in the multi-line input box.

### Key Functions

- `GetProcessName(pid)`: Returns the name of the process with the given PID.
    ```lua
    local pid = 1234
    local name = GetProcessName(pid)
    print("Process name: " .. name)
    ```

- `GetProcessesNames()`: Returns a table of the names of all running processes.
    ```lua
    local names = GetProcessesNames()
    for i, name in ipairs(names) do
        print("Process " .. i .. ": " .. name)
    end
    ```

- `GetProcessIdByName(name)`: Returns the PID of the process with the given name.
    ```lua
    local name = "my_process"
    local pid = GetProcessIdByName(name)
    print("PID: " .. pid)
    ```

- `GetProcessBaseAddress(pid)`: Returns the base address of the process with the given PID.
    ```lua
    local pid = 1234
    local baseAddress = GetProcessBaseAddress(pid)
    print("Base address: " .. baseAddress)
    ```

- `GetLoadedModules(pid)`: Returns a table of the names of all modules loaded by the process with the given PID.
    ```lua
    local pid = 1234
    local modules = GetLoadedModules(pid)
    for i, module in ipairs(modules) do
        print("Module " .. i .. ": " .. module)
    end
    ```

- `read(pid, address, size)`: Reads `size` bytes of memory from the process with the given PID at the given address. Returns a string containing the read bytes, or an error message if the read operation failed.
    ```lua
    local pid = 1234
    local address = 0x12345678
    local size = 4
    local data = read(pid, address, size)
    if data then
        print("Memory contents: " .. data)
    else
        print("Failed to read memory")
    end
    ```

- `write(pid, address, data)`: Writes data to memory in the process with the given PID at the given address. The data should be a string where each character represents a byte. Returns true if the write operation was successful, or an error message if it failed.
   ```lua
    local pid = 1234
    local address = 0x12345678 
    local data = "\x01\x02\x03\x04"
    if write(pid, address, data) then
        print("Successfully wrote to memory")
    else
        print("Failed to write to memory")
    end
    ```