#include "InputManager.h"

#include <imgui-SFML.h>
#include <iostream>

#include "../GameEngine.h"
#include "imgui.h"

InputManager::InputManager(sf::RenderWindow &window, GameEngine &gameEngine)
    : _window(window), _gameEngine(gameEngine) {
}

void InputManager::RegisterInput(const size_t scene,
                                 const sf::Keyboard::Key key, std::shared_ptr<InputAction> action) {
    _actionKeyMaps[scene][key] = action;
}

void InputManager::RegisterInput(const size_t scene,
                                 const sf::Mouse::Button btn, std::shared_ptr<InputAction> action) {
    _actionMouseBtnMaps[scene][btn] = action;
}

void InputManager::RegisterInput(const size_t scene,
                                 const sf::Mouse::Wheel wheel, std::shared_ptr<InputAction> action) {
    _actionMouseWheelMaps[scene][wheel] = action;
}

void InputManager::RegisterInput(const size_t scene, const MouseMove mv,
                                 std::shared_ptr<InputAction> action) {
    _actionMouseMoveMaps[scene][mv] = action;
}

bool InputManager::ProcessInput(const size_t scene) {
    // Reset End actions from previous frame to None
    if (_actionKeyMaps.count(scene)) {
        for (auto &[key, action]: _actionKeyMaps[scene]) {
            if (action->Type() == ActionType::End)
                action->Type() = ActionType::None;
        }
    }

    if (_actionMouseBtnMaps.count(scene)) {
        for (auto &[btn, action]: _actionMouseBtnMaps[scene]) {
            if (action->Type() == ActionType::End)
                action->Type() = ActionType::None;
        }
    }

    if (_actionMouseWheelMaps.count(scene)) {
        for (auto &[wheel, action]: _actionMouseWheelMaps[scene]) {
            if (action->Type() == ActionType::End)
                action->Type() = ActionType::None;
        }
    }

    if (_actionMouseMoveMaps.count(scene)) {
        for (auto &[mv, action]: _actionMouseMoveMaps[scene]) {
            if (action->Type() == ActionType::End)
                action->Type() = ActionType::None;
        }
    }

    // Process window events
    while (const auto event = _window.pollEvent()) {
        ImGui::SFML::ProcessEvent(_window, *event);

        if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            if (_actionKeyMaps[scene].contains(keyPressed->code)) {
                auto &action = _actionKeyMaps[scene][keyPressed->code];
                if (action->Type() != ActionType::Start) {
                    action->Type() = ActionType::Start;
                    action->Value() = 1;
                }
            }
        } else if (const auto *keyReleased = event->getIf<sf::Event::KeyReleased>()) {
            if (_actionKeyMaps[scene].contains(keyReleased->code)) {
                auto &action = _actionKeyMaps[scene][keyReleased->code];
                action->Type() = ActionType::End;
                action->Value() = 0;
            }
        } else if (const auto *mouseBtnPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (_actionMouseBtnMaps[scene].contains(mouseBtnPressed->button)) {
                auto &action = _actionMouseBtnMaps[scene][mouseBtnPressed->button];
                if (action->Type() != ActionType::Start) {
                    action->Type() = ActionType::Start;
                    action->Value() = 1;
                }
            }
        } else if (const auto *mouseBtnReleased = event->getIf<sf::Event::MouseButtonReleased>()) {
            if (_actionMouseBtnMaps[scene].contains(mouseBtnReleased->button)) {
                auto &action = _actionMouseBtnMaps[scene][mouseBtnReleased->button];
                action->Type() = ActionType::End;
                action->Value() = 0;
            }
        } else if (const auto *mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
            if (_actionMouseMoveMaps[scene].contains(MouseMove::Move)) {
                auto &action = _actionMouseMoveMaps[scene][MouseMove::Move];
                action->Type() = ActionType::Start;
                action->Value2() = sf::Vector2i(mouseMoved->position.x, mouseMoved->position.y);
            }
        } else if (const auto *mouseWheel = event->getIf<sf::Event::MouseWheelScrolled>()) {
            if (_actionMouseWheelMaps[scene].contains(mouseWheel->wheel)) {
                auto &action = _actionMouseWheelMaps[scene][mouseWheel->wheel];
                action->Type() = ActionType::Start;
                action->Value() = static_cast<short>(mouseWheel->delta > 0 ? 1 : -1);
            }
        } else if (const auto *closed = event->getIf<sf::Event::Closed>()) {
            _window.close();
            return false;
        }
    }

    return true;
}
