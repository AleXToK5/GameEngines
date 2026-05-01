#ifndef SPRITERENDERSYSTEM_H
#define SPRITERENDERSYSTEM_H

#include <SFML/Graphics.hpp>
#include "../../Ecs/Systems/ISystem.h"
#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"
#include "../../GameEngine/Assets/AssetManager.h"
#include "../Physics/TransformComponent.h"
#include "SpriteComponent.h"

class SpriteRenderSystem final : public ISystem
{
    sf::RenderWindow& _window;
    const AssetManager& _assets;
    ComponentStorage<TransformComponent>& _transforms;
    ComponentStorage<SpriteComponent>& _sprites;
    Filter _filter;

public:
    SpriteRenderSystem(World& world, sf::RenderWindow& window, const AssetManager& assets)
        : ISystem(world),
          _window(window),
          _assets(assets),
          _transforms(world.GetStorage<TransformComponent>()),
          _sprites(world.GetStorage<SpriteComponent>()),
          _filter(FilterBuilder(world)
              .With<TransformComponent>()
              .With<SpriteComponent>()
              .Build())
    {}

    void OnInit() override {}

    void OnUpdate() override
    {
        for (int e : _filter)
        {
            auto& t = _transforms.Get(e);
            auto& s = _sprites.Get(e);

            const sf::Texture& tex = _assets.GetTexture(s.TextureName);
            sf::Sprite sprite(tex);
            sprite.setPosition({t.X, t.Y});
            sprite.setScale({t.ScaleX, t.ScaleY});
            _window.draw(sprite);
        }
    }
};

#endif //SPRITERENDERSYSTEM_H