#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>

#include "imgui.h"
#include "imgui-SFML.h"

#include "Object.h"
#include "Logo.h"
#include "Text.h"
#include "Gui.h"
#include "ConfigReader.h"

class Window {
    sf::RenderWindow _window;
    sf::Clock        _deltaClock;
    bool             _isRun   = true;
    bool             _isPaused = false;

    std::shared_ptr<Logo> _logo;
    std::shared_ptr<Text> _text;
    sf::String            _savedText;
    Gui                   _gui;

    void Initialize();
    void UpdateUserInput();
    void UpdateLogic(float deltaTime);
    void UpdateGui();
    void Render();

    void SetPaused(bool paused);

public:
    Window(unsigned int wWidth, unsigned int wHeight);
    void Run();
};

#endif //WINDOW_H