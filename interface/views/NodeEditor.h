/*
* Code for the node editor.
* - views/NodeEditor.h
*/

#include <imnodes/imnodes.h>

// TODO: Implement a Proximity-View-Like interface.
// UNUSED

struct Node {
    int id;
    std::string title;
    ImVec2 pos;
    std::string type;
    std::vector<std::string> input_names;
    std::vector<std::string> output_names;
};

std::vector<Node> nodes;
bool NewNodeWindow = false;
char NewNodeType[128] = "";
int NewNodeInputs = 1;
int NewNodeOutputs = 1;
std::vector<char*> NewNodeINames;
std::vector<char*> NewNodeONames;

void ui::views::NodeEditor() {
    
    if (!states::running["Node Editor"])
        return;

	ImGui::Begin(
		"Node Editor",
		&states::running["Node Editor"],
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_HorizontalScrollbar |
        ImGuiWindowFlags_NoSavedSettings
	);

    ImNodes::BeginNodeEditor();

    if (ImGui::IsWindowFocused() && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_N))) {
        NewNodeWindow = true;
    }

    if (NewNodeWindow) {
        ImGui::Begin("New Node", &NewNodeWindow);

        ImGui::BeginChild("New Node Child", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysUseWindowPadding);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 12.0f);

        float windowWidth = ImGui::GetWindowWidth();
        float textWidth = ImGui::CalcTextSize("Node Configuration").x;
        ImGui::SetCursorPosX((windowWidth - textWidth) / 2);
        ImGui::Text("Node Configuration");

        ImGui::Separator();
        ImGui::Spacing();
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 8));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 8));
        ImGui::InputInt("Input Count", &NewNodeInputs);
        ImGui::InputInt("Output Count", &NewNodeOutputs);
        ImGui::PopStyleVar(2);
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Text("Input Names");
        ImGui::Separator();
        NewNodeINames.resize(NewNodeInputs);
        for (int i = 0; i < NewNodeInputs; i++) {
            char label[32];
            sprintf(label, "Input %d", i + 1);
            if (NewNodeINames[i] == nullptr) {
                NewNodeINames[i] = new char[128];
                strcpy(NewNodeINames[i], "Input");
            }
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 8));
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 8));
            ImGui::InputText(label, NewNodeINames[i], 128);
            ImGui::PopStyleVar(2);
        }
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Text("Output Names");
        ImGui::Separator();
        NewNodeONames.resize(NewNodeOutputs);
        for (int i = 0; i < NewNodeOutputs; i++) {
            char label[32];
            sprintf(label, "Output %d", i + 1);
            if (NewNodeONames[i] == nullptr) {
                NewNodeONames[i] = new char[128];
                strcpy(NewNodeONames[i], "Output");
            }
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 8));
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 8));
            ImGui::InputText(label, NewNodeONames[i], 128);
            ImGui::PopStyleVar(2);
        }
        ImGui::Spacing();

        // make button wider
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 8));
        float buttonWidth = ImGui::CalcTextSize("Create").x + ImGui::GetStyle().FramePadding.x * 2;
        ImGui::SetCursorPosX((ImGui::GetWindowContentRegionWidth() - buttonWidth) / 2);
        if (ImGui::Button("Create")) {
            std::vector<std::string> input_names(NewNodeInputs);
            for (int i = 0; i < NewNodeInputs; i++) {
                input_names[i] = NewNodeINames[i];
            }

            std::vector<std::string> output_names(NewNodeOutputs);
            for (int i = 0; i < NewNodeOutputs; i++) {
                output_names[i] = NewNodeONames[i];
            }

            nodes.push_back({ (int)nodes.size(), "New Node", ImGui::GetMousePos(), "", input_names, output_names });
            NewNodeWindow = false;
        }
        ImGui::PopStyleVar();
        ImGui::EndChild();
        ImGui::End();
    }


    for (auto& node : nodes) {
        ImNodes::BeginNode(node.id);

        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted(node.title.c_str());
        ImNodes::EndNodeTitleBar();

        for (int i = 0; i < node.input_names.size(); i++) {
            ImNodes::BeginInputAttribute(node.id * 100 + i);
            ImGui::TextUnformatted(node.input_names[i].c_str());
            ImNodes::EndInputAttribute();
        }

        for (int i = 0; i < node.output_names.size(); i++) {
            ImNodes::BeginOutputAttribute(node.id * 100 + i + (int)node.input_names.size());
            ImGui::TextUnformatted(node.output_names[i].c_str());
            ImNodes::EndOutputAttribute();
        }

        ImNodes::EndNode();
    }

    ImNodes::EndNodeEditor();

	ImGui::End();
}