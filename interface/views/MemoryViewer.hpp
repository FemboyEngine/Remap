/*
* Remap
* A memory editor, written in C++ and ImGui.
* With a focus on usability and performance.
*
* This file is part of Remap.
* - views/processes.hpp
*
* Code for memory viewer window.
*/

#include <Zydis/Zydis.h>
#include <map>

static char buffer[MAX_BUFFER_SIZE];

SIZE_T bytesRead;
SIZE_T size;
LPCVOID base = nullptr;

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

	if (base == nullptr)
	{
		base = (LPCVOID)GetProcessBaseAddress(state::pid);
		size = sizeof(buffer);

		ReadProcessMemory(state::CurrentProcess, base, buffer, size, &bytesRead);

		std::cout << "Invoked" << std::endl;
	}

    // use a flag to prevent the disassembler from being invoked every frame
    // it fucking works without the flag, idk why the fuck it fails here with the flag
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
            ZyanStatus status = ZydisDecoderDecodeFull(&decoder, &buffer[i], size - i, &instruction, operands);
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

        ImGuiListClipper clipper;
        clipper.Begin(disasmText.size());
        while (clipper.Step()) {
            for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
            {
                uint64_t relative = disasmText[i].first + (uint64_t)base;
                ImGui::Text("%016llX  %s", relative, disasmText[i].second.c_str());

                // select
            }
        }

        ImGui::EndChild();
    }

	ImGui::End();
}