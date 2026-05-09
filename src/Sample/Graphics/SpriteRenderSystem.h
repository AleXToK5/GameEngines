#ifndef SPRITERENDERSYSTEM_H
#define SPRITERENDERSYSTEM_H

#include <SFML/Graphics.hpp>
#include <cmath>
#include "../../Ecs/Systems/ISystem.h"
#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"
#include "../../GameEngine/Assets/AssetManager.h"
#include "../Physics/TransformComponent.h"
#include "../Physics/ColliderComponent.h"
#include "SpriteComponent.h"
#include "RenderSettingsComponent.h"

class SpriteRenderSystem final : public ISystem {
    sf::RenderWindow &_window;
    const AssetManager &_assets;

    ComponentStorage<TransformComponent> &_transforms;
    ComponentStorage<SpriteComponent> &_sprites;
    ComponentStorage<ColliderComponent> &_colliders;
    ComponentStorage<RenderSettingsComponent> &_settings;

    Filter _spriteFilter;
    Filter _colliderFilter;
    Filter _settingsFilter;

    void DrawLine(const sf::Vector2f &p1, const sf::Vector2f &p2) {
        sf::Vertex line[] = {sf::Vertex(p1), sf::Vertex(p2)};
        _window.draw(line, 2, sf::PrimitiveType::Lines);
    }

public:
    SpriteRenderSystem(World &world, sf::RenderWindow &window, const AssetManager &assets)
        : ISystem(world),
          _window(window),
          _assets(assets),
          _transforms(world.GetStorage<TransformComponent>()),
          _sprites(world.GetStorage<SpriteComponent>()),
          _colliders(world.GetStorage<ColliderComponent>()),
          _settings(world.GetStorage<RenderSettingsComponent>()),
          _spriteFilter(FilterBuilder(world).With<TransformComponent>().With<SpriteComponent>().Build()),
          _colliderFilter(FilterBuilder(world).With<TransformComponent>().With<ColliderComponent>().Build()),
          _settingsFilter(FilterBuilder(world).With<RenderSettingsComponent>().Build()) {
    }

    void OnInit() override {
    }

    void OnUpdate() override {
        // Получаем настройки рендера
        RenderSettingsComponent *currentSettings = nullptr;
        for (int e: _settingsFilter) {
            currentSettings = &_settings.Get(e);
            break;
        }

        if (!currentSettings) return;

        // 1. Отрисовка Текстур
        if (currentSettings->DrawTextures) {
            for (int e: _spriteFilter) {
                auto &t = _transforms.Get(e);
                auto &s = _sprites.Get(e);

                const sf::Texture &tex = _assets.GetTexture(s.TextureName);
                sf::Sprite sprite(tex);
                // Опционально: отцентровать спрайт
                // sprite.setOrigin(tex.getSize().x / 2.0f, tex.getSize().y / 2.0f);
                sprite.setPosition({t.X, t.Y});
                sprite.setScale({t.ScaleX, t.ScaleY});
                _window.draw(sprite);
            }
        }

        // 2. Отрисовка Коллайдеров
        if (currentSettings->DrawColliders) {
            for (int e: _colliderFilter) {
                auto &bbox = _colliders.Get(e);
                auto &t = _transforms.Get(e);

                // Рисуем RectangleShape (предполагаем, что у ColliderComponent есть Size)
                sf::RectangleShape rect;
                rect.setSize(sf::Vector2f(bbox.Size.x - 1.f, bbox.Size.y - 1.f));
                rect.setOrigin({bbox.Size.x / 2.f, bbox.Size.y / 2.f});
                rect.setPosition({t.X, t.Y});
                rect.setFillColor(sf::Color(0, 0, 0, 0)); // Прозрачный внутри
                rect.setOutlineColor(sf::Color::Red);
                rect.setOutlineThickness(1.5f);
                _window.draw(rect);
            }
        }

        // 3. Отрисовка Сетки
        if (currentSettings->DrawGrid) {
            const sf::Font &font = _assets.GetFont("BaseFont");
            sf::Text gridText(font, "", 12);
            gridText.setFillColor(sf::Color(255, 255, 255, 150));

            const int gridSize = 64;
            sf::View view = _window.getView();
            float leftX = view.getCenter().x - view.getSize().x / 2.0f;
            float rightX = leftX + view.getSize().x + gridSize;

            float nextGridX = leftX - std::fmod(leftX, (float) gridSize);
            if (nextGridX < leftX) nextGridX += gridSize;

            for (float x = nextGridX; x < rightX; x += gridSize) {
                DrawLine({x, 0}, {x, view.getSize().y});
            }

            for (float y = 0; y <= view.getSize().y; y += gridSize) {
                DrawLine({leftX, view.getSize().y - y}, {rightX, view.getSize().y - y});

                for (float x = nextGridX; x < rightX; x += gridSize) {
                    int xCell = static_cast<int>(x) / gridSize;
                    int yCell = static_cast<int>(y) / gridSize;
                    gridText.setString("(" + std::to_string(xCell) + "," + std::to_string(yCell) + ")");
                    gridText.setPosition({x + 3.f, view.getSize().y - y - gridSize + 2.f});
                    _window.draw(gridText);
                }
            }
        }
    }
};

#endif //SPRITERENDERSYSTEM_H
