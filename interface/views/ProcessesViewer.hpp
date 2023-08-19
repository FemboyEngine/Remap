/*
* Remap
* A memory editor, written in C++ and ImGui.
* With a focus on usability and performance.
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


char searchBuffer[MAX_SEARCH_BUFFER_SIZE];
std::string selected{};

std::vector<std::string> processes = GetProcessesNames();

void ui::views::Processes() noexcept
{
	ImGui::Begin(
		"Remap",
		&state::isRunning,
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_HorizontalScrollbar
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

		// click to select
		if (ImGui::IsItemClicked())
		{
			selected = processes[i];
			state::pid = GetProcessIdByName(selected.c_str());
			state::CurrentProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, state::pid);
			state::BaseAddress = (LPCVOID)GetProcessBaseAddress(state::pid);

			// read process memory and store it in state::memory vector
			MEMORY_BASIC_INFORMATION mbi;
			std::vector<uint8_t> buffer;
			char* p = 0;
			while (VirtualQueryEx(state::CurrentProcess, p, &mbi, sizeof(mbi))) {
				if (mbi.State == MEM_COMMIT && (mbi.Protect & PAGE_GUARD) == 0 && mbi.Protect != PAGE_NOACCESS) {
					std::vector<uint8_t> tempBuffer(mbi.RegionSize);
					SIZE_T bytesRead;
					if (ReadProcessMemory(state::CurrentProcess, p, &tempBuffer[0], mbi.RegionSize, &bytesRead)) {
						buffer.insert(buffer.end(), tempBuffer.begin(), tempBuffer.end());
					}
				}
				p += mbi.RegionSize;
			}
			state::memory = buffer;

		}
	}

	if (state::CurrentProcess != NULL && state::pid != 0)
	{
		ImGui::OpenPopup("Success");

		if (ImGui::BeginPopupModal("Success", &state::popup, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Successfully opened process %s with pid %d", selected.c_str(), state::pid);
		}

		ImGui::EndPopup();
	}

	ImGui::EndChild();

	ImGui::End();
}