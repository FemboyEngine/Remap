/*
* Code for disassembly window.
* - views/Disassembly.h
*/

#include "zydis_wrapper/zydis_wrapper.h"
#include <map>

class DisassemblyView : public View {
public:
    DisassemblyView() : View("Disassembly") {}

protected:
    /**
     * Render the disassembly view.
     * Disassembles the binary code, displays it in a tabular format
     */
    void Content() override {
        if (state::current_process == NULL && state::pid == 0) return;

        if (state::disassembled == false) {
            disasmText.clear();
            disasmText.reserve(size / 16);

            int i = 0;

            while (i < size)
            {
                if (zydis.Disassemble(runtime_address + offset + i, &state::memory[i], size - i))
                {
                    std::string instruction = zydis.GetInstruction();
                    std::string bytes = zydis.GetInstructionBytes(&state::memory[i]);

                    //printf("INS: %s\t%s\n", instruction, bytes);

                    // | address | bytes | disassembly |
                    disasmText.emplace_back(runtime_address + offset + i, instruction, bytes);
                }
                i += zydis.GetDecodedInstruction().length;
            }

            state::disassembled = true;
        }

        if (state::disassembled) {
            ImGui::BeginChild("Disassembly", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);

            // column headers
            ImGui::Columns(3);
            ImGui::Text("Address"); ImGui::NextColumn();
            ImGui::Text("Bytes"); ImGui::NextColumn();
            ImGui::Text("Disassembly"); ImGui::NextColumn();
            ImGui::Separator();

            ImGuiListClipper clipper;
            clipper.Begin((int)disasmText.size());

            std::string name = remap::GetProcessName(state::pid);

            while (clipper.Step()) {
                for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
                {
                    uint64_t address = std::get<0>(disasmText[i]);
                    std::string disassembly = std::get<1>(disasmText[i]);
                    std::string bytes = std::get<2>(disasmText[i]);

                    // address column
                    std::string relative = std::format("{}+{:X}", name, address);

                    ImGui::Selectable(relative.c_str(), false, ImGuiSelectableFlags_SpanAllColumns);
                    ImGui::NextColumn();

                    // bytes column
                    ImGui::Text(bytes.c_str());
                    ImGui::NextColumn();

                    // disassembly column
                    ImVec4 blue = ImVec4(0.54f, 0.71f, 0.98f, 1.0f);

                    std::string instruction = disassembly.substr(0, disassembly.find(" "));
                    ImGui::TextColored(blue, instruction.c_str());
                    ImGui::SameLine();
                    ImGui::Text(disassembly.substr(instruction.length()).c_str());
                    ImGui::NextColumn();

                    if (ImGui::IsItemHovered()) {
                        ImGui::SetTooltip("%s\t%s", relative.c_str(), disassembly.c_str());
                    }
                }
            }

            ImGui::Columns(1);
            ImGui::EndChild();
        }
    }

private:
    static const int kMaxBufferSize = 1024 * 1024 * 15;
    char buffer[kMaxBufferSize];
    SIZE_T size = sizeof(buffer);
    uint64_t runtime_address = 0;
    uint64_t offset = 0;
    std::vector<std::tuple<uint64_t, std::string, std::string>> disasmText;

    Zydis zydis;
};
