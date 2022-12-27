#include "App.h"

int main() {

    sf::RenderWindow window{ sf::VideoMode(640, 480), "L-System Editor" };
    App myApp(&window);
    window.setFramerateLimit(60);

    ImGui::SFML::Init(window);

    auto& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    sf::Clock deltaClock{};
    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());
        ImGui::DockSpaceOverViewport();

        myApp.renderUI();
        myApp.renderImage();
    }

    ImGui::SFML::Shutdown();

    return 0;
}