/*
* Code for loaded modules viewer window.
* - views/Modules.h
*/

#include <vector>
#include <TlHelp32.h>

class ModulesView : public View {
public:
	ModulesView() : View("Modules") {}

protected:
	/**
	 * Render the loaded modules viewer window.
	 * Lists and displays the loaded modules of a selected process
	 */
	void Content() override {
		if (state::current_process == NULL && state::pid == 0) return;

		modules.clear();
		modules = remap::GetLoadedModules(state::pid);

		for (auto& module : modules)
		{
			ImGui::Text("%s", module.c_str());
		}

		ImGui::End();
	}

private:
	std::vector<std::string> modules;
};