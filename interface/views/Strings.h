/*
* Code for strings viewer window
* - views/Strings.h
*/

class StringsView : public View {
public:
    StringsView() : View("Strings") {}

protected:
    /**
     * Render the strings viewer window.
     * Displays and manages strings within the memory of a selected process
     */
    void Content() override {
        if (state::current_process == NULL && state::pid == 0) return;

        if (state::mapped_strings == false) {
            strings.clear();

            std::stringstream ss;
            for (const auto& ch : state::memory) {
                if (ch == '\n') {
                    if (!ss.str().empty()) {
                        strings.push_back(ss.str());
                        ss.str(std::string());
                    }
                }
                else if (isprint(ch)) {
                    ss << ch;
                }
            }
            if (!ss.str().empty()) {
                strings.push_back(ss.str());
            }

            state::mapped_strings = true;
        }

        if (state::mapped_strings) {
            ImGuiListClipper clipper;
            clipper.Begin(static_cast<int>(strings.size()));

            while (clipper.Step()) {
                for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {
                    ImGui::Text("%s", strings[i].c_str());
                }
            }
        }
    }

private:
    std::vector<std::string> strings;
};