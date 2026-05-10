#include <iostream>
#include "GameEngine/GameEngine.h"
#include "GameEngine/GameEngineConfiguration.h"
#include "Sample/Scenes/MenuScene.h"

int main() {
    setlocale(LC_ALL, "");

    try {
        GameEngine engine{GameEngineConfiguration{}};
        engine.LoadScene<MenuScene>(engine);
        engine.Run();
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
