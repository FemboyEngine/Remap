/*
* Remap
* A memory editor, written in C++ and ImGui.
* With a focus on usability and performance.
*
* This file is part of Remap.
* - views/MemoryViewer.hpp
*
* Code for memory viewer window.
*/

#include <Zydis/Zydis.h>
#include <iomanip>
#include <sstream>
#include <map>

static char buffer[MAX_BUFFER_SIZE];

SIZE_T bytesRead;
SIZE_T size = sizeof(buffer);

uint64_t runtime_address = 0;
uint64_t offset = 0;

std::vector<std::pair<ZyanU64, std::string>> disasmText;

void ui::views::MemoryViewer() noexcept
{
	ImGui::Begin(
        "Memory Viewer",
        &state::isRunning,
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_HorizontalScrollbar
    );

	if (state::CurrentProcess == NULL && state::pid == 0) return;

	ImGui::Text("Process: %s", selected.c_str());
	ImGui::Text("PID: %d", state::pid);

    if (state::memory.size() == 0) {
		state::memory.resize(size);
		ReadProcessMemory(state::CurrentProcess, state::BaseAddress, state::memory.data(), size, &bytesRead);
	}

    // use a flag to prevent the disassembler from being invoked every frame
    if (state::disassembled == false) {

        ZydisDecoder decoder;
        ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_STACK_WIDTH_64);

        ZydisFormatter formatter;
        ZydisFormatterInit(&formatter, ZYDIS_FORMATTER_STYLE_INTEL);

        int i = 0;

        while (i < size)
        {
            ZydisDecodedInstruction instruction;
            ZydisDecodedOperand operands[ZYDIS_MAX_OPERAND_COUNT];
            ZyanStatus status = ZydisDecoderDecodeFull(&decoder, &state::memory[i], size - i, &instruction, operands);
            if (ZYAN_SUCCESS(status))
            {
                char buffer[256];
                ZydisFormatterFormatInstruction(&formatter, &instruction, operands, ZYDIS_MAX_OPERAND_COUNT, buffer,
                    sizeof(buffer), runtime_address + offset + i, NULL);
                
                disasmText.push_back(std::make_pair(runtime_address + offset + i, buffer));
            }
            i += instruction.length;

        }

        std::cout << "invoked" << std::endl;
        state::disassembled = true;
    }

    if (state::disassembled) {
        ImGui::Text("Disassembled");
        ImGui::BeginChild("Disassembly", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);

        // column headers
        ImGui::Columns(3);
        ImGui::Text("Address"); ImGui::NextColumn();
        ImGui::Text("Bytes"); ImGui::NextColumn();
        ImGui::Text("Disassembly"); ImGui::NextColumn();
        ImGui::Separator();

        // disassembly rows
        ImGuiListClipper clipper;
        clipper.Begin(disasmText.size());
        while (clipper.Step()) {
            for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
            {
                std::string name = GetProcessName(state::pid);
                uint64_t relative = disasmText[i].first;

                std::string bytes = "..."; // implement it later
                std::string disassembly = disasmText[i].second;

                // to show bytes
                std::stringstream ss;
                for (int i = 0; i < 16; i++) {
                    ss << std::hex << std::uppercase << std::setw(2)
                        << std::setfill('0') << (int)state::memory[relative + i] << " ";
                }
                bytes = ss.str();

                // address column
                ImGui::Text("%s+%llX", name.c_str(), relative); ImGui::NextColumn();

                // bytes column
                ImGui::Text(bytes.c_str()); ImGui::NextColumn();

                // disassembly column
                ImVec4 cyan = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);
                //ImGui::Text(disassembly.c_str()); ImGui::NextColumn();
                // color instructions cyan, and the rest white
                std::string instruction = disassembly.substr(0, disassembly.find(" "));
                ImGui::TextColored(cyan, instruction.c_str());
                ImGui::SameLine();
                ImGui::Text(disassembly.substr(instruction.length()).c_str()); ImGui::NextColumn();

                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("%s", disassembly.c_str());
                }

                // click to copy
                if (ImGui::IsItemClicked()) {
                    ImGui::SetClipboardText(disassembly.c_str());
                }
            }
        }

        ImGui::Columns(1);
        ImGui::EndChild();
    }

	ImGui::End();
}