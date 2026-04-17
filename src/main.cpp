#include <iostream>
#include <SFML/Graphics.hpp>
#include <imgui-SFML.h> // Обязательно подключаем ImGui-SFML

#include "Ecs/Systems/SystemsManager.h"
#include "Ecs/World/World.h"

#include "Sample/Core/InitSystem.h"
#include "Sample/Core/RestartSystem.h"
#include "Sample/Input/InputSystem.h"
#include "Sample/Physics/MovementSystem.h"
#include "Sample/Physics/CollisionSystem.h"
#include "Sample/Physics/BoundarySystem.h"
#include "Sample/Gameplay/Player/PlayerShootSystem.h"
#include "Sample/Gameplay/Enemies/AsteroidSpawnerSystem.h"
#include "Sample/Graphics/RenderSystem.h"
#include "Sample/UI/UiSystem.h"

#include "ConfigReader.h"

int main() {
    setlocale(LC_ALL, "");

    try {
        ConfigReader config("config.json");

        const unsigned int wWidth = config.GetWindowWidth();
        const unsigned int wHeight = config.GetWindowHeight();
        sf::RenderWindow window(sf::VideoMode({wWidth, wHeight}), "Entity Component System Test");
        window.setFramerateLimit(60);

        if (!ImGui::SFML::Init(window)) {
            std::cerr << "Failed to initialize ImGui-SFML!" << std::endl;
            return -1;
        }

        World world;
        SystemsManager systems(world);

        systems.AddInitializer(std::make_shared<InitSystem>(world, config));
        systems.AddSystem(std::make_shared<InputSystem>(world, window));
        systems.AddSystem(std::make_shared<MovementSystem>(world, config));
        systems.AddSystem(std::make_shared<RenderSystem>(world, window, config.GetMainFontPath()));
        systems.AddSystem(std::make_shared<PlayerShootSystem>(world, config));
        systems.AddSystem(std::make_shared<BoundarySystem>(world, config));
        systems.AddSystem(std::make_shared<AsteroidSpawnerSystem>(world, config));
        systems.AddSystem(std::make_shared<CollisionSystem>(world));
        systems.AddSystem(std::make_shared<RestartSystem>(world, config));
        systems.AddSystem(std::make_shared<UiSystem>(world));

        sf::Clock deltaClock;

        while (window.isOpen()) {
            while (const std::optional event = window.pollEvent()) {
                ImGui::SFML::ProcessEvent(window, *event);

                if (event->is<sf::Event::Closed>()) {
                    window.close();
                }
            }

            ImGui::SFML::Update(window, deltaClock.restart());

            window.clear(sf::Color::Black);

            systems.Update();

            ImGui::SFML::Render(window);

            window.display();
        }

        ImGui::SFML::Shutdown();
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
