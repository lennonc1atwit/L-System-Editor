#include "App.h"

typedef std::pair<sf::Vector2<float>, float> DrawRecord;

App::App(sf::RenderWindow* win) {
    mAxiom = "";
    mSymbols.clear();
    mRules.clear();
    mRWin = win;
    mIterations = 0;
}

App::~App() {

}

void App::generateSystem() {
    std::string str = mAxiom;
    std::string new_str = "";
    for (std::size_t i = 0; i < mIterations; ++i) {
        new_str = "";
        // Go through our string
        for (char c : str) {
            // Check if this char has a rule associated with it
            if (mRules.find(c) != mRules.end()) 
                new_str += mRules[c];
            else 
                new_str += c;
        }
        str = new_str;
    }

    mSystemString = str;
}

void App::showMainMenuBar() {
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Save", "CTRL+S")) {}
            if (ImGui::MenuItem("Open", "CTRL+O")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void App::popupSymbol() {
    ImGuiWindowFlags win_flags = ImGuiWindowFlags_NoScrollbar || ImGuiWindowFlags_AlwaysAutoResize || ImGuiWindowFlags_NoResize;
    ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue;
    char buff[8] = { 0 };

    if (ImGui::BeginPopupModal("Add Symbol", &symbolPopupOpen, win_flags)) {
        ImGui::Text("Add Symbol");
        if (ImGui::InputText("new symbols", buff, 2, flags)) {
            char c = buff[0];
            mSymbols.insert(c);
            printf("Symbol Added: %c\n", c);
        }
        if (ImGui::Button("Close##symbol")) {
            symbolPopupOpen = false;
        }
        ImGui::EndPopup();
    }

    ImGui::OpenPopup("Add Symbol");
}

void App::popupRule() {
    ImGuiWindowFlags win_flags = ImGuiWindowFlags_NoScrollbar || ImGuiWindowFlags_AlwaysAutoResize || ImGuiWindowFlags_NoResize;
    if (ImGui::BeginPopupModal("Add Rule", &rulePopupOpen, win_flags)) {
        bool rule_is_valid = true;

        ImGui::Text("Add Rule");
        // Get char that we are replacing
        static char a_buff[2] = ""; ImGui::InputText("char", a_buff, 2);
        // Get replacement
        static char b_buff[64] = ""; ImGui::InputText("replacement", b_buff, 64);

        // Validate that rule fits language
        if (ImGui::Button("Create Rule")) {
            char c = a_buff[0];
            std::string r = b_buff;

            if (mSymbols.find(c) == mSymbols.end()) {
                rule_is_valid = false;
                printf("ERROR: Invalid character %c in rule.\n", c);
            }

            for (int i = 0; i < IM_ARRAYSIZE(b_buff); i++) {
                if (b_buff[i] == '\0')
                    break;

                if (mSymbols.find(b_buff[i]) == mSymbols.end()) {
                    rule_is_valid = false;
                    printf("ERROR: Invalid replacment %c in rule at %d.\n", c, i);
                    break;
                }
            }

            if (rule_is_valid) {
                mRules.insert({ c, r });
                printf("Rule Added: %c -> %s\n", c, r.c_str());
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Close##rule")) {
            rulePopupOpen = false;
        }
        ImGui::EndPopup();
    }

    ImGui::OpenPopup("Add Rule");
}

void App::renderUI() {
    showMainMenuBar();

    if (ImGui::Begin("Language Description")) {
        char erase = NULL;

        /*
       *  Symbols
       *  @ TODO Gui finishes
       */
        ImGui::Text("Symbols");
        ImGui::SameLine(ImGui::GetWindowWidth()-30);
        if (ImGui::Button("+##symbol")) {
            symbolPopupOpen = true;
        }
        for (auto symbol : mSymbols) {
            ImGui::Text("%c", symbol);
            ImGui::SameLine(ImGui::GetWindowWidth() - 30);

            ImGui::PushID(symbol);
            if (ImGui::Button("-##symbol")) { erase = symbol; }
            ImGui::PopID();
        }

        if (erase) {
            printf("erasing symbol %c\n", erase);
            mSymbols.erase(erase);
        }

        ImGui::Separator();

        /*
        * Rules
        * @ TODO Clear input after a making rule
        * @ TODO Gui finishes
        */
        ImGui::Text("Rules");
        ImGui::SameLine(ImGui::GetWindowWidth() - 30);
        if (ImGui::Button("+##rule")) { rulePopupOpen = true; }

        erase = NULL;
        for (auto rule : mRules) {
            char c = rule.first;
            std::string s = rule.second;

            ImGui::Text("%c -> %s",c, s.c_str());
            ImGui::SameLine(ImGui::GetWindowWidth() - 30);

            ImGui::PushID(c);
            if (ImGui::Button("-##rule")) { erase = c; }
            ImGui::PopID();
        }

        if (erase) {
            printf("erasing rule for %c\n", erase);
            mRules.erase(erase);
        }

        ImGui::End();
    }

    /*
    * System String
    * @ TODO fix ??? on display
    * @ TODO
    * @ TODO Gui finishes
    */
    if (ImGui::Begin("String")) {
        char axi_buf[15] = { 0 };

        /*
        * Axiom
        * @ TODO Gui finishes
        */
        if (ImGui::InputText("Edit Axiom", axi_buf, IM_ARRAYSIZE(axi_buf), ImGuiInputTextFlags_EnterReturnsTrue)) {
            bool axiom_is_valid = true;
            for (std::size_t i = 0; i < IM_ARRAYSIZE(axi_buf); ++i) {
                char c = axi_buf[i];
                // null byte check
                if (c == '\0')
                    break;

                if (mSymbols.find(c) == mSymbols.end()) {
                    axiom_is_valid = false;
                    printf("ERROR: Invalid Axiom character %c at %d (%s)\n", c, i, axi_buf);
                    break;
                }
            }

            if (axiom_is_valid) {
                printf("Axiom updated: %s -> %s\n", mAxiom.c_str(), axi_buf);
                mAxiom = axi_buf;
                generateSystem();
            }
        }

        if (ImGui::Button("+##iter")) { 
            if(mIterations < 1000)
                mIterations++; 
            generateSystem(); 
        }
        ImGui::SameLine();
        if (ImGui::Button("-##iter")) {
            if(mIterations > 0)
                mIterations--; 
            generateSystem(); 
        }
        ImGui::SameLine();
        ImGui::SliderInt("1000", &mIterations, 0, 1000, "%d iterations");
        ImGui::TextWrapped("%s", mSystemString.c_str(), ImGui::GetWindowWidth() - 30);
    }
    ImGui::End();

    if (ImGui::Begin("Representation")) {
       
        static ImVec4 clr { 1.0f, 0.0f, 1.0f, 0.5f };
        ImGuiColorEditFlags flags = ImGuiColorEditFlags_AlphaBar;
        //ImGui::ColorPicker4("Branch Color", (float*)&clr, flags, nullptr);
        //ImGui::ColorPicker4("Leaf Color", (float*)&clr, flags, nullptr);
    }
    ImGui::End();

    if (ImGui::Begin("Viewport")) {
        viewportSize = ImGui::GetWindowSize();
        ImGui::Image(mRt);
    }
    ImGui::End();

    if (symbolPopupOpen)
        popupSymbol();

    if (rulePopupOpen)
        popupRule();
}

void App::renderImage() {
    mRt.create(viewportSize.x, viewportSize.y);
    mRt.clear(sf::Color::Black);

    if (!mSystemString.empty()) {
        sf::VertexArray line_segment(sf::LineStrip);
        std::stack <DrawRecord> history;

        sf::Vector2<float> pos = sf::Vector2<float>(ImGui::GetWindowWidth() / 2, ImGui::GetWindowHeight());
        float dir = -90;

        line_segment.append(pos);

        for (std::size_t i = 0; i < mSystemString.length(); ++i) {
            
            switch (mSystemString[i])
            {
            case 'F': { // Forward
                sf::Vector2<float> new_pt = line_segment[line_segment.getVertexCount() - 1].position + (sf::Vector2<float>(
                    cos(dir * M_PI / 180.f), 
                    sin(dir * M_PI / 180.f))
                * 10.f);

                line_segment.append(sf::Vertex(new_pt));
                pos = new_pt;
                break; }
            case '[': {
                DrawRecord record;
                record.first = pos;
                record.second = dir;
                history.push(record);
                break; }
            case ']': {
                mRt.draw(line_segment);
                line_segment.clear();
                DrawRecord record = history.top();
                pos = record.first;
                dir = record.second;
                line_segment.append(pos);
                history.pop();
                break; }
            case '+': {// Right 90 deg CW
                dir += 22.5f;
                break; }
            case '-': {// Left  90 deg CCW
                dir -= 22.5f;
                break; }
            default:
                break;
            }
        }
        mRt.draw(line_segment);
    }

    mRWin->clear();
    ImGui::SFML::Render(*mRWin);
    mRWin->display();
}
