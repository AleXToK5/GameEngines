#include "Scene.h"
#include "GameEngine.h"

Scene::Scene(GameEngine& gameEngine)
    : gameEngine(gameEngine), systemsManager(world) {}

std::shared_ptr<InputAction> Scene::RegisterAction(const std::string &name)
{
    auto action = std::make_shared<InputAction>(name);
    actionMap[name] = action;
    return action;
}

void Scene::RegisterAction(const sf::Keyboard::Key key, const std::string& name)
{
    const auto action = RegisterAction(name);
    gameEngine.RegisterInput(key, action);
}

void Scene::RegisterAction(const sf::Mouse::Button btn, const std::string &name)
{
    const auto action = RegisterAction(name);
    gameEngine.RegisterInput(btn, action);
}

void Scene::RegisterAction(const sf::Mouse::Wheel wheel, const std::string &name)
{
    const auto action = RegisterAction(name);
    gameEngine.RegisterInput(wheel, action);
}

void Scene::RegisterAction(const MouseMove mv, const std::string &name)
{
    const auto action = RegisterAction(name);
    gameEngine.RegisterInput(mv, action);
}

