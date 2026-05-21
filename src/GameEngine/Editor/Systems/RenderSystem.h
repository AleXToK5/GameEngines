#pragma once
#include <SFML/Graphics.hpp>
#include "../../../Ecs/Systems/ISystem.h"
#include "../../../Ecs/Filter/FilterBuilder.h"
#include "../../../GameEngine/Assets/AssetManager.h"
#include "../../../Sample/Physics/TransformComponent.h"
#include "../../../Sample/Graphics/SpriteComponent.h"
#include "../../../Sample/Graphics/AnimatorComponent.h"
#include "../RenderState.h"
#include "../RenderService.h"
#include "../CameraService.h"

class EditorRenderSystem final : public ISystem {
    sf::RenderWindow& _window;
    const AssetManager& _assets;
    RenderState& _state;

    ComponentStorage<TransformComponent>& _transforms;
    ComponentStorage<SpriteComponent>& _sprites;
    ComponentStorage<AnimatorComponent>& _animators;

    Filter _spriteFilter;
    sf::Vector2i _mousePixel;

public:
    EditorRenderSystem(World& world, sf::RenderWindow& window,
                       const AssetManager& assets, RenderState& state)
        : ISystem(world),
          _window(window), _assets(assets), _state(state),
          _transforms(world.GetStorage<TransformComponent>()),
          _sprites(world.GetStorage<SpriteComponent>()),
          _animators(world.GetStorage<AnimatorComponent>()),
          _spriteFilter(FilterBuilder(world).With<TransformComponent>().With<SpriteComponent>().Build()) {}

    void SetMousePixel(const sf::Vector2i& mp) { _mousePixel = mp; }

    void OnInit() override {}

    void OnUpdate() override {
        sf::View editorView = CameraService::BuildView(_window, _state);
        _window.setView(editorView);

        RenderService::DrawGrid(_window, _state);

        for (int e : _spriteFilter) {
            const auto& t = _transforms.Get(e);
            const auto& s = _sprites.Get(e);
            if (s.TextureName.empty()) continue;

            sf::Color tint = sf::Color::White;
            if (e == _state.HeldEntity)
                tint = sf::Color(255, 255, 100, 220);
            else if (_state.Selection.count(e))
                tint = sf::Color(100, 200, 255, 220);

            try {
                if (_animators.Has(e)) {
                    const auto& anim = _animators.Get(e);
                    const Animation& animData = _assets.GetAnimation(anim.CurrentAnimation);
                    int fw = animData.Size().x, fh = animData.Size().y;
                    sf::Sprite sprite(animData.GetTexture(),
                        sf::IntRect({anim.CurrentFrame * fw, 0}, {fw, fh}));
                    sprite.setOrigin({fw / 2.f, fh / 2.f});
                    sprite.setPosition({t.X, t.Y});
                    sprite.setScale({t.ScaleX, t.ScaleY});
                    sprite.setColor(tint);
                    _window.draw(sprite);
                } else {
                    const sf::Texture& tex = _assets.GetTexture(s.TextureName);
                    sf::Sprite sprite(tex);
                    sprite.setOrigin({tex.getSize().x / 2.f, tex.getSize().y / 2.f});
                    sprite.setPosition({t.X, t.Y});
                    sprite.setScale({t.ScaleX, t.ScaleY});
                    sprite.setColor(tint);
                    _window.draw(sprite);
                }

                if (_state.Selection.count(e)) {
                    sf::RectangleShape outline;
                    float hw = EditorConstants::TileSize / 2.f;
                    float hh = EditorConstants::TileSize / 2.f;
                    outline.setSize({hw * 2.f, hh * 2.f});
                    outline.setOrigin({hw, hh});
                    outline.setPosition({t.X, t.Y});
                    outline.setFillColor(sf::Color::Transparent);
                    outline.setOutlineColor(sf::Color(100, 200, 255, 200));
                    outline.setOutlineThickness(2.f);
                    _window.draw(outline);
                }
            } catch (...) {}
        }

        RenderService::DrawPreview(_window, _state, _assets, _mousePixel);
        _window.setView(_window.getDefaultView());
    }
};