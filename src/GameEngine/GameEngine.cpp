#include "GameEngine.h"

#include <imgui-SFML.h>
#include <imgui.h>

void GameEngine::Render()
{
    // Очистка и фон выполняются в Scene::Render()
    if (_scenes.count(_currentScene))
    {
        _scenes[_currentScene]->Render(_window);
    }
    else
    {
        _window.clear(sf::Color::Black);
    }

    ImGui::SFML::Render(_window);
    _window.display();
}

GameEngine::GameEngine(const GameEngineConfiguration& config)
    : _config(config), _isRunning(false), _currentScene(0)
{
    _window.create(
        sf::VideoMode({_config.width, _config.height}),
        "Game Engine");

    _window.setFramerateLimit(60);

    if (!ImGui::SFML::Init(_window))
        throw std::runtime_error("Failed to initialize ImGui-SFML");

    _inputManager = std::make_shared<InputManager>(_window, *this);

    _assetManager.LoadFromFile(_config.assetsFile);
}

void GameEngine::Initialize()
{
    _isRunning = true;
}

void GameEngine::Run()
{
    Initialize();

    while (_window.isOpen() && _isRunning)
    {
        float deltaTime = _deltaClock.restart().asSeconds();

        _inputManager->ProcessInput(_currentScene);

        if (_scenes.count(_currentScene))
            _scenes[_currentScene]->Update(deltaTime);

        // Применяем отложенную смену сцены ПОСЛЕ Update
        if (_pendingSceneChange)
        {
            _pendingSceneChange();
            _pendingSceneChange = nullptr;
        }

        ImGui::SFML::Update(_window, sf::milliseconds(static_cast<int32_t>(deltaTime * 1000.f)));
        Render();
    }

    Quit();
}

void GameEngine::Quit()
{
    ImGui::SFML::Shutdown();
    _window.close();
    _isRunning = false;
}