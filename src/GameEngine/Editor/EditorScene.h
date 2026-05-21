#pragma once

#ifdef EDITOR_ENABLED

#include <iostream>
#include <functional>
#include "../../GameEngine/Scene.h"
#include "../../GameEngine/GameEngine.h"
#include "../../Sample/Level/LevelInitializer.h"
#include "../../Sample/Graphics/AnimationSystem.h"

#include "RenderState.h"
#include "CameraService.h"
#include "GameObjectController.h"
#include "JsonSerializer.h"

#include "Systems/RenderSystem.h"
#include "Systems/InputSystem.h"
#include "Systems/MouseInputSystem.h"
#include "Systems/GuiSystem.h"
#include "Systems/EntityGuiSystem.h"

class EditorScene final : public Scene {
    static constexpr sf::Color BgColor{0x55, 0x55, 0xAA, 0xFF};

    std::function<void()> _onQuit;

    RenderState _state;
    GameObjectController _controller;

    std::shared_ptr<EditorRenderSystem> _renderSystem;

    float _windowWidth;
    float _windowHeight;

public:
    explicit EditorScene(GameEngine& engine, std::function<void()> onQuit = nullptr)
        : Scene(engine),
          _onQuit(onQuit),
          _controller(world, static_cast<float>(engine.Window().getSize().y)),
          _windowWidth(static_cast<float>(engine.Window().getSize().x)),
          _windowHeight(static_cast<float>(engine.Window().getSize().y)) {}

    void Init() override {
        _state.CameraCenter = {_windowWidth / 2.f, _windowHeight / 2.f};

        systemsManager.AddInitializer(std::make_shared<LevelInitializer>(
            world, "level.json", "config.json",
            gameEngine.Assets(), _windowHeight, true));

        RegisterAction(sf::Keyboard::Key::Left,    "CamLeft");
        RegisterAction(sf::Keyboard::Key::Right,   "CamRight");
        RegisterAction(sf::Keyboard::Key::Up,      "CamUp");
        RegisterAction(sf::Keyboard::Key::Down,    "CamDown");
        RegisterAction(sf::Keyboard::Key::Escape,  "Escape");

        RegisterAction(sf::Mouse::Button::Left,    "LMB");
        RegisterAction(sf::Mouse::Button::Right,   "RMB");
        RegisterAction(sf::Mouse::Button::Middle,  "MMB");
        RegisterAction(sf::Mouse::Wheel::Vertical, "Wheel");
        RegisterAction(Move,                       "MouseMove");

        auto inputSys = std::make_shared<EditorInputSystem>(
            world, _state,
            actionMap["CamLeft"], actionMap["CamRight"],
            actionMap["CamUp"],   actionMap["CamDown"]);

        auto mouseSys = std::make_shared<EditorMouseInputSystem>(
            world, gameEngine.Window(), _state, _controller,
            actionMap["LMB"], actionMap["RMB"], actionMap["MMB"],
            actionMap["Wheel"], actionMap["MouseMove"]);

        _renderSystem = std::make_shared<EditorRenderSystem>(
            world, gameEngine.Window(), gameEngine.Assets(), _state);

        auto guiSys       = std::make_shared<EditorGuiSystem>(world, gameEngine.Assets(), _state);
        auto entityGuiSys = std::make_shared<EditorEntityGuiSystem>(world);
        auto animSys      = std::make_shared<AnimationSystem>(world, gameEngine.Assets());

        systemsManager.AddSystem(inputSys);
        systemsManager.AddSystem(mouseSys);
        systemsManager.AddSystem(_renderSystem);
        systemsManager.AddSystem(guiSys);
        systemsManager.AddSystem(entityGuiSys);
        systemsManager.AddSystem(animSys);

        systemsManager.Initialize();
    }

    void Update(float delta) override {
        if (actionMap.count("Escape") && actionMap["Escape"]->Type() == ActionType::Start) {
            actionMap["Escape"]->Type() = ActionType::None;
            if (_onQuit) _onQuit();
            return;
        }

        if (_renderSystem && actionMap.count("MouseMove"))
            _renderSystem->SetMousePixel(actionMap["MouseMove"]->Value2());

        if (_state.SaveRequested) {
            JsonSerializer::SaveLevel("level.json", world, _windowHeight);
            _state.SaveRequested = false;
        }
    }

    void Render(sf::RenderWindow& window) override {
        window.clear(BgColor);
        systemsManager.Update();
    }
};

#endif