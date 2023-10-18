/*
* Code for disassembly window.
* - views/Disassembly.h
*/

#include <Zydis/Zydis.h>
#include <iomanip>
#include <sstream>
#include <map>
#include <format>

class DisassemblyView : public View {
public:
    DisassemblyView() : View("Disassembly") {}

protected:
    void Content() override {
        if (state::current_process == NULL && state::pid == 0) return;

        if (state::disassembled == false) {
            ZydisDecoder decoder;
            ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_STACK_WIDTH_64);

            ZydisFormatter formatter;
            ZydisFormatterInit(&formatter, ZYDIS_FORMATTER_STYLE_INTEL);

            disasmText.clear();

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

                    std::string bytes;
                    for (int j = 0; j < instruction.length; j++)
                    {
                        bytes += std::format("{:02x} ", static_cast<int>(state::memory[i + j]));
                    }

                    // | address | bytes | disassembly |
                    disasmText.emplace_back(runtime_address + offset + i, buffer, bytes);
                }
                i += instruction.length;

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
            while (clipper.Step()) {
                for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
                {
                    std::string name = remap::GetProcessName(state::pid);

                    uint64_t address = std::get<0>(disasmText[i]);
                    std::string disassembly = std::get<1>(disasmText[i]);
                    std::string bytes = std::get<2>(disasmText[i]);

                    // address column
                    std::stringstream ss2;
                    ss2 << std::uppercase << std::hex << address;
                    std::string relative = name + "+" + ss2.str();

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
    static const int kMaxBufferSize = 1024;
    char buffer[kMaxBufferSize];
    SIZE_T size = sizeof(buffer);
    uint64_t runtime_address = 0;
    uint64_t offset = 0;
    std::vector<std::tuple<uint64_t, std::string, std::string>> disasmText;
};
