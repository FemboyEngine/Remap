/*
* Code for processes window.
* - views/Processes.h
*/

#include "views.h"
#include <core/memory.h>
#include <core/state.h>

#include <iostream>

class ProcessesView : public View {
public:
    ProcessesView() : View("Processes") {}

protected:
    /**
     * Render the processes window.
     * Displays a list of processes and allows users to search for a specific process,
     * and selects a process to allocate memory for future operations
     */
    void Content() override {
        ImGui::SetCursorPosX((ImGui::GetWindowContentRegionWidth() - 15) / 2);

        ImGui::Text("Processes");
        ImGui::PushItemWidth(ImGui::GetWindowContentRegionWidth());
        ImGui::InputText("##search", searchBuffer, IM_ARRAYSIZE(searchBuffer));
        ImGui::PopItemWidth();
        ImGui::BeginChild("##processes", ImVec2(ImGui::GetWindowContentRegionWidth(), 0), true);

        std::string input_lower = searchBuffer;
        std::transform(input_lower.begin(), input_lower.end(), input_lower.begin(), ::tolower);

        for (auto& process : processes)
        {
            std::string process_lower = process;
            std::transform(process_lower.begin(), process_lower.end(), process_lower.begin(), ::tolower);

            if (process_lower.find(input_lower) != std::string::npos)
            {
                ImGui::Text(process.c_str());
            }

            // Process selected, allocate memory
            if (ImGui::IsItemClicked())
            {
                selected = process;
                state::pid = remap::GetProcessIdByName(selected.c_str());
                state::current_process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, state::pid);
                state::base_address = (LPCVOID)remap::GetProcessBaseAddress(state::pid);

                MEMORY_BASIC_INFORMATION mbi;
                std::vector<uint8_t> buffer = {};
                buffer.reserve(1024 * 1024 * 100); // reserve 100 MB upfront
                char* p = 0;
                constexpr size_t bufferSize = 1024 * 1024 * 10;

                while (VirtualQueryEx(state::current_process, p, &mbi, sizeof(mbi))) {
                    if (mbi.State == MEM_COMMIT && (mbi.Protect & PAGE_GUARD) == 0 && mbi.Protect != PAGE_NOACCESS) {
                        for (size_t offset = 0; offset < mbi.RegionSize; offset += bufferSize) {
                            size_t bytesToRead = (std::min)(bufferSize, mbi.RegionSize - offset);
                            SIZE_T bytesRead;
                            buffer.resize(bytesToRead);
                            if (ReadProcessMemory(state::current_process, p + offset, buffer.data(), bytesToRead, &bytesRead)) {
                                buffer.insert(buffer.end(), buffer.begin(), buffer.begin() + bytesRead);
                            }
                        }
                    }
                    p += mbi.RegionSize;
                }

                state::memory = std::move(buffer);

                // reset flags
                state::disassembled = false;
                state::mapped_strings = false;
            }
        }

        ImGui::EndChild();
    }

private:
    char searchBuffer[1024] = "";
    std::string selected;
    std::vector<std::string> processes = remap::GetProcessesNames();
};
