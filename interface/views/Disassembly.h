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
			StartDisassembly();
		}

		if (popup) {
			ShowPopup();
		}

		if (state::disassembled) {
			DisplayDisassembly();
		}
	}

private:
	void StartDisassembly() {
		ImGui::OpenPopup("Disassembling...");
		std::thread disassemblyThread(&DisassemblyView::Disassemble, this);
		disassemblyThread.detach();

		state::disassembled = true;
	}

	void ShowPopup() {
		if (ImGui::BeginPopupModal("Disassembling...", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			SetStyle();

			ImGuiStyle& style = ImGui::GetStyle();

			ImGui::Text("Disassembly is being processed.\n");

			if (ImGui::Button("Close")) {
				popup = false;

				style = orig_style;
			}

			ImGui::EndPopup();
		}
	}

	void SetStyle() {
		ImGuiStyle& style = ImGui::GetStyle();

		style.WindowPadding = ImVec2(20, 20);
		style.WindowRounding = 10.0f;
		style.FramePadding = ImVec2(10, 10);
		style.FrameRounding = 5.0f;
		style.ItemSpacing = ImVec2(10, 5);
	}

	void DisplayDisassembly() {
		std::lock_guard<std::mutex> lock(disasmMutex);

		ImGui::BeginChild("Disassembly", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);

		DisplayColumnHeaders();

		ImGuiListClipper clipper;
		clipper.Begin(static_cast<int>(disasmText.size()));

		std::string name = remap::GetProcessName(state::pid);

		while (clipper.Step()) {
			for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
			{
				DisplayRow(i, name);
			}
		}

		ImGui::Columns(1);
		ImGui::EndChild();
	}

	void DisplayColumnHeaders() {
		ImGui::Columns(3);
		ImGui::Text("Address"); ImGui::NextColumn();
		ImGui::Text("Bytes"); ImGui::NextColumn();
		ImGui::Text("Disassembly"); ImGui::NextColumn();
		ImGui::Separator();
	}

	void DisplayRow(int i, const std::string& name) {
		auto [address, disassembly, bytes] = disasmText[i];

		std::string relative = std::format("{}+{:X}", name, address);

		DisplayAddressColumn(address, name);
		DisplayBytesColumn(bytes);
		DisplayDisassemblyColumn(disassembly);

		if (ImGui::IsItemHovered()) {
			ImGui::SetTooltip("%s\t%s", relative.c_str(), disassembly.data());
		}
	}

	void DisplayAddressColumn(uint64_t address, const std::string& name) {
		std::string relative = std::format("{}+{:X}", name, address);

		ImGui::Selectable(relative.c_str(), false, ImGuiSelectableFlags_SpanAllColumns);
		ImGui::NextColumn();
	}

	void DisplayBytesColumn(const std::string& bytes) {
		ImGui::Text(bytes.data());
		ImGui::NextColumn();
	}

	void DisplayDisassemblyColumn(const std::string& disassembly) {
		ImVec4 blue = ImVec4(0.54f, 0.71f, 0.98f, 1.0f);

		std::string_view instruction = disassembly.substr(0, disassembly.find(" "));
		ImGui::TextColored(blue, instruction.data());
		ImGui::SameLine();
		ImGui::Text(disassembly.data() + instruction.length());
		ImGui::NextColumn();
	}

	void Disassemble() {
		std::vector<Disassembly> disasmTextTemp;
		disasmTextTemp.reserve(size / 16);

		int i = 0;

		while (i < size)
		{
			if (zydis.Disassemble(runtime_address + offset + i, &state::memory[i], size - i))
			{
				std::string instruction = zydis.GetInstruction();
				std::string bytes = zydis.GetInstructionBytes(&state::memory[i]);

				Disassembly disassembly;
				disassembly.address = runtime_address + offset + i;
				disassembly.disassembly = instruction;
				disassembly.bytes = bytes;

				disasmTextTemp.push_back(disassembly);
			}
			i += zydis.GetDecodedInstruction().length;
		}

		std::lock_guard<std::mutex> lock(disasmMutex);
		disasmText.swap(disasmTextTemp);
	}

	struct Disassembly {
		uint64_t address;
		std::string disassembly;
		std::string bytes;
	};

	static const int kMaxBufferSize = 1024 * 1024 * 15;
	char buffer[kMaxBufferSize];
	SIZE_T size = sizeof(buffer);
	uint64_t runtime_address = 0;
	uint64_t offset = 0;
	std::vector<Disassembly> disasmText;
	std::mutex disasmMutex;

	bool popup = true;

	Zydis zydis;
	ImGuiStyle orig_style = ImGui::GetStyle();
};
