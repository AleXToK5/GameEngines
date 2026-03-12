#include "Window.h"

Window::Window(unsigned int wWidth, unsigned int wHeight)
{
    _window.create(sf::VideoMode({wWidth, wHeight}), "Demo");

    auto desktop = sf::VideoMode::getDesktopMode();
    _window.setPosition({
        (int)(desktop.size.x / 2 - wWidth / 2),
        (int)(desktop.size.y / 2 - wHeight / 2)
    });

    _window.setFramerateLimit(60);

    if (!ImGui::SFML::Init(_window)) {
        std::cerr << "Failed to init ImGui-SFML\n";
        exit(-1);
    }

    ImGui::GetStyle().ScaleAllSizes(1.0f);
    ImGui::GetIO().FontGlobalScale = 2.0f;

    Initialize();
}

void Window::Initialize()
{
    ConfigReader config("config.json");
    auto windowSize = _window.getSize();

    // Загружаем все пути текстур
    std::vector<std::filesystem::path> paths;
    for (int i = 0; i < config.GetLogoCount(); i++)
        paths.push_back(config.GetLogoPath(i));

    // Один логотип со всеми текстурами
    _logo = std::make_shared<Logo>(
        paths,
        sf::Vector2f(100.f, 100.f),
        sf::Vector2f(config.GetVelocityX(), config.GetVelocityY()),
        config.GetScale(),
        config.GetColor(0)
    );

    _text = std::make_shared<Text>(config.GetFontPath(), L"Токарев Александр ИУ3-22М", 24);
    _text->SetPosition({
        0.f,
        (float)windowSize.y - (float)_text->GetCharacterSize()
    });
    _savedText = L"Токарев Александр ИУ3-22М";
}

void Window::SetPaused(bool paused)
{
    _isPaused = paused;

    if (_isPaused) {
        // Сохраняем текущий текст и показываем "Пауза" по центру
        _text->SetString(L"\u041f\u0430\u0443\u0437\u0430");  // "Пауза" в unicode
        _text->CenterIn(_window.getSize());
    } else {
        // Возвращаем старый текст на прежнее место
        _text->SetString(_savedText);
        auto windowSize = _window.getSize();
        _text->SetPosition({
            0.f,
            (float)windowSize.y - (float)_text->GetCharacterSize()
        });
    }
}

void Window::Run()
{
    while (_isRun)
    {
        sf::Time delta = _deltaClock.restart();
        ImGui::SFML::Update(_window, delta);

        UpdateUserInput();
        UpdateLogic(delta.asSeconds());
        UpdateGui();
        Render();
    }

    _window.close();
    ImGui::SFML::Shutdown();
}

void Window::UpdateUserInput()
{
    while (const std::optional event = _window.pollEvent())
    {
        ImGui::SFML::ProcessEvent(_window, *event);

        if (event->is<sf::Event::Closed>())
            _isRun = false;

        if (const auto* key = event->getIf<sf::Event::KeyPressed>())
        {
            if (key->code == sf::Keyboard::Key::Space)
                SetPaused(!_isPaused);
        }
    }
}

void Window::UpdateLogic(float deltaTime)
{
    if (!_isPaused)
        _logo->Update(deltaTime, _window.getSize());
}

void Window::UpdateGui()
{
    _gui.Draw(_logo, _text);
}

void Window::Render()
{
    _window.clear();

    if (!_isPaused)
        _logo->Draw(_window);

    _text->Draw(_window);

    ImGui::SFML::Render(_window);
    _window.display();
}