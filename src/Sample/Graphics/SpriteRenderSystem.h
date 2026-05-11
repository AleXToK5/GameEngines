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
#include "AnimatorComponent.h"
#include "../Gameplay/Environment/DecorComponent.h" // <-- ДОБАВИЛИ ИНКЛУД

class SpriteRenderSystem final : public ISystem {
    World &_world; // <-- ДОБАВИЛИ ССЫЛКУ НА ДВИЖОК
    sf::RenderWindow &_window;
    const AssetManager &_assets;

    ComponentStorage<TransformComponent> &_transforms;
    ComponentStorage<SpriteComponent> &_sprites;
    ComponentStorage<ColliderComponent> &_colliders;
    ComponentStorage<RenderSettingsComponent> &_settings;
    ComponentStorage<AnimatorComponent> &_animators;

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
          _world(world), // <-- ИНИЦИАЛИЗИРУЕМ
          _window(window),
          _assets(assets),
          _transforms(world.GetStorage<TransformComponent>()),
          _sprites(world.GetStorage<SpriteComponent>()),
          _colliders(world.GetStorage<ColliderComponent>()),
          _settings(world.GetStorage<RenderSettingsComponent>()),
          _animators(world.GetStorage<AnimatorComponent>()),
          _spriteFilter(FilterBuilder(world).With<TransformComponent>().With<SpriteComponent>().Build()),
          _colliderFilter(FilterBuilder(world).With<TransformComponent>().With<ColliderComponent>().Build()),
          _settingsFilter(FilterBuilder(world).With<RenderSettingsComponent>().Build()) {
    }

    void OnInit() override {
    }

    void OnUpdate() override {
        RenderSettingsComponent *currentSettings = nullptr;
        for (int e: _settingsFilter) {
            currentSettings = &_settings.Get(e);
            break;
        }

        if (!currentSettings) return;

        // 1. Отрисовка Текстур
        if (currentSettings->DrawTextures) {
            auto drawEntity = [&](int e) {
                auto &t = _transforms.Get(e);

                float hw, hh;

                if (_animators.Has(e)) {
                    auto &anim = _animators.Get(e);
                    const Animation &animData = _assets.GetAnimation(anim.CurrentAnimation);

                    const sf::Texture &tex = animData.GetTexture();
                    int fw = animData.Size().x;
                    int fh = animData.Size().y;

                    sf::IntRect frameRect({anim.CurrentFrame * fw, 0}, {fw, fh});
                    sf::Sprite sprite(tex, frameRect);

                    hw = fw / 2.0f;
                    hh = fh / 2.0f;

                    sprite.setOrigin({hw, hh});
                    sprite.setPosition({t.X, t.Y});
                    sprite.setScale({t.ScaleX, t.ScaleY});
                    _window.draw(sprite);
                } else {
                    auto &s = _sprites.Get(e);

                    if (s.TextureName.empty()) return;
                    const sf::Texture &tex = _assets.GetTexture(s.TextureName);
                    sf::Sprite sprite(tex);

                    hw = tex.getSize().x / 2.0f;
                    hh = tex.getSize().y / 2.0f;

                    sprite.setOrigin({hw, hh});
                    sprite.setPosition({t.X, t.Y});
                    sprite.setScale({t.ScaleX, t.ScaleY});
                    _window.draw(sprite);
                }
            };

            for (int e: _spriteFilter) {
                if (_world.GetStorage<DecorComponent>().Has(e)) {
                    drawEntity(e);
                }
            }

            for (int e: _spriteFilter) {
                if (!_world.GetStorage<DecorComponent>().Has(e)) {
                    drawEntity(e);
                }
            }
        }

        // 2. Отрисовка Коллайдеров
        if (currentSettings->DrawColliders) {
            for (int e: _colliderFilter) {
                auto &bbox = _colliders.Get(e);
                auto &t = _transforms.Get(e);

                if (bbox.Type == ColliderType::AABB) {
                    sf::RectangleShape rect;
                    rect.setSize(sf::Vector2f(bbox.Size.x - 1.f, bbox.Size.y - 1.f));
                    rect.setOrigin({bbox.Size.x / 2.f, bbox.Size.y / 2.f});
                    rect.setPosition({t.X, t.Y});
                    rect.setFillColor(sf::Color::Transparent);
                    rect.setOutlineColor(sf::Color::Red);
                    rect.setOutlineThickness(1.5f);
                    _window.draw(rect);
                } else if (bbox.Type == ColliderType::Circle) {
                    sf::CircleShape circle;
                    circle.setRadius(bbox.Radius);
                    circle.setOrigin({bbox.Radius, bbox.Radius});
                    circle.setPosition({t.X, t.Y});
                    circle.setFillColor(sf::Color::Transparent);
                    circle.setOutlineColor(sf::Color::Red);
                    circle.setOutlineThickness(1.5f);
                    _window.draw(circle);
                }
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

            float nextGridX = leftX - std::fmod(leftX, static_cast<float>(gridSize));
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
