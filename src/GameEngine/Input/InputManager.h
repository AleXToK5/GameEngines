#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H
#include <SFML/Graphics.hpp>
#include <memory>
#include <unordered_map>

#include "InputAction.h"

enum MouseMove { Move };

class GameEngine;

class InputManager {
    sf::RenderWindow &_window;
    GameEngine &_gameEngine;

    std::unordered_map<size_t, std::unordered_map<sf::Keyboard::Key, std::shared_ptr<InputAction> > > _actionKeyMaps;
    std::unordered_map<size_t, std::unordered_map<sf::Mouse::Button, std::shared_ptr<InputAction> > >
    _actionMouseBtnMaps;
    std::unordered_map<size_t, std::unordered_map<sf::Mouse::Wheel, std::shared_ptr<InputAction> > >
    _actionMouseWheelMaps;
    std::unordered_map<size_t, std::unordered_map<MouseMove, std::shared_ptr<InputAction> > > _actionMouseMoveMaps;

public:
    InputManager(sf::RenderWindow &window, GameEngine &gameEngine);

    void RegisterInput(size_t scene,
                       sf::Keyboard::Key key, std::shared_ptr<InputAction> action);

    void RegisterInput(size_t scene,
                       sf::Mouse::Button btn, std::shared_ptr<InputAction> action);

    void RegisterInput(size_t scene,
                       sf::Mouse::Wheel wheel, std::shared_ptr<InputAction> action);

    void RegisterInput(size_t scene,
                       MouseMove mv, std::shared_ptr<InputAction> action);

    bool ProcessInput(size_t scene);
};

#endif //INPUTMANAGER_H
