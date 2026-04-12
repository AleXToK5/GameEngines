#include <iostream>
#include <SFML/Graphics.hpp>

#include "Ecs/Systems/SystemsManager.h"
#include "Ecs/World/World.h"

#include "Sample/Systems/InitSystem.h"
#include "Sample/Systems/InputSystem.h"
#include "Sample/Systems/MovementSystem.h"
#include "Sample/Systems/RenderSystem.h"

#include "ConfigReader.h"

int main() {
    // Пример использования
    setlocale(LC_ALL, "");

    try {
        ConfigReader config("config.json");

        const unsigned int wWidth = config.GetWindowWidth();
        const unsigned int wHeight = config.GetWindowHeight();
        sf::RenderWindow window(sf::VideoMode({wWidth, wHeight}), "Entity Component System Test");
        window.setFramerateLimit(60);

        World world;
        SystemsManager systems(world);
        
        systems.AddInitializer(std::make_shared<InitSystem>(world));
        systems.AddSystem(std::make_shared<InputSystem>(world, window));
        systems.AddSystem(std::make_shared<MovementSystem>(world));
        systems.AddSystem(std::make_shared<RenderSystem>(world, window, config.GetMainFontPath()));

        while (window.isOpen()) {
            systems.Update();
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}