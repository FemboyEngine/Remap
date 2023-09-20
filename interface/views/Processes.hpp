/*
* Remap
* A memory editor, written in C++ and ImGui.
*
* This file is part of Remap.
* - views/processes.hpp
*
* Code for processes window.
*/

#include "views.h"
#include <core/memory.hpp>
#include <core/state.h>
#include <iostream>

char searchBuffer[kMaxBufferSize];
std::string selected{};

std::vector<std::string> processes = remap::GetProcessesNames();

void ui::views::Processes() {
	if (!states::running["Processes"])
		return;

	ImGui::Begin(
		"Remap",
		&states::running["Processes"],
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_HorizontalScrollbar |
		ImGuiWindowFlags_NoSavedSettings
	);

	// center text
	ImGui::SetCursorPosX((ImGui::GetWindowContentRegionWidth() - 15) / 2);

	ImGui::Text("Processes");
	ImGui::PushItemWidth(ImGui::GetWindowContentRegionWidth());
	ImGui::InputText("##search", searchBuffer, IM_ARRAYSIZE(searchBuffer));
	ImGui::PopItemWidth();
	ImGui::BeginChild("##processes", ImVec2(ImGui::GetWindowContentRegionWidth(), 0), true);

	std::string input_lower = searchBuffer;
	std::transform(input_lower.begin(), input_lower.end(), input_lower.begin(), ::tolower);

	for (int i = 0; i < processes.size(); i++)
	{
		std::string process_lower = processes[i];
		std::transform(process_lower.begin(), process_lower.end(), process_lower.begin(), ::tolower);

		if (strstr(process_lower.c_str(), input_lower.c_str()))
		{
			ImGui::Text(processes[i].c_str());
		}

		if (ImGui::IsItemClicked())
		{
			selected = processes[i];
			state::pid = remap::GetProcessIdByName(selected.c_str());
			state::CurrentProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, state::pid);
			state::BaseAddress = (LPCVOID)remap::GetProcessBaseAddress(state::pid);

			// temporary
			MEMORY_BASIC_INFORMATION mbi;
			std::vector<uint8_t> buffer;
			char* p = 0;
			constexpr size_t bufferSize = 1024 * 1024; // 1 MB
			std::vector<uint8_t> tempBuffer(bufferSize);
			while (VirtualQueryEx(state::CurrentProcess, p, &mbi, sizeof(mbi))) {
				if (mbi.State == MEM_COMMIT && (mbi.Protect & PAGE_GUARD) == 0 && mbi.Protect != PAGE_NOACCESS) {
					SIZE_T bytesRead;
					for (size_t offset = 0; offset < mbi.RegionSize; offset += bufferSize) {
						size_t bytesToRead = (std::min)(bufferSize, mbi.RegionSize - offset);
						if (ReadProcessMemory(state::CurrentProcess, p + offset, &tempBuffer[0], bytesToRead, &bytesRead)) {
							buffer.insert(buffer.end(), tempBuffer.begin(), tempBuffer.begin() + bytesRead);
						}
					}
				}
				p += mbi.RegionSize;
			}
			state::memory = buffer;
			state::disassembled = false; // reset disassembled flag
			state::mapped_strings = false;
		}
	}

	if (state::CurrentProcess != NULL && state::pid != 0)
	{
		ImGui::OpenPopup("Success");

		if (ImGui::BeginPopupModal("Success", &state::popup, 
			ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings))
		{
			ImGui::Text("Successfully opened process %s with pid %d", selected.c_str(), state::pid);
		}

		ImGui::EndPopup();
	}

	ImGui::EndChild();

	ImGui::End();
}