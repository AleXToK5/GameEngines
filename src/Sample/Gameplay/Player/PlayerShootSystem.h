#ifndef PLAYERSHOOTSYSTEM_H
#define PLAYERSHOOTSYSTEM_H

#include <memory>
#include "../../../Ecs/Systems/ISystem.h"
#include "../../../Ecs/Filter/FilterBuilder.h"
#include "../../../GameEngine/Input/InputAction.h"
#include "../../../GameEngine/Assets/AssetManager.h"
#include "../../Physics/TransformComponent.h"
#include "../../Physics/VelocityComponent.h"
#include "../../Physics/ColliderComponent.h"
#include "../../Graphics/SpriteComponent.h"
#include "PlayerComponent.h"
#include "../Weapons/ProjectileComponent.h"

class PlayerShootSystem final : public ISystem {
    World &_world;
    const AssetManager &_assets;
    std::shared_ptr<InputAction> _shootAction;
    ComponentStorage<TransformComponent> &_transforms;
    ComponentStorage<PlayerComponent> &_players;
    Filter _filter;

public:
    PlayerShootSystem(World &world, const AssetManager &assets, std::shared_ptr<InputAction> shootAction)
        : ISystem(world),
          _world(world),
          _assets(assets),
          _shootAction(shootAction),
          _transforms(world.GetStorage<TransformComponent>()),
          _players(world.GetStorage<PlayerComponent>()),
          _filter(FilterBuilder(world).With<TransformComponent>().With<PlayerComponent>().Build()) {
    }

    void OnInit() override {
    }

    void OnUpdate() override {
        for (int e: _filter) {
            auto &player = _players.Get(e);

            if (player.ShootCooldown > 0) {
                player.ShootCooldown--;
            }

            if (_shootAction->Type() == ActionType::Start && player.ShootCooldown <= 0) {
                auto &t = _transforms.Get(e);

                int bullet = _world.CreateEntity();

                float startX = t.X + (t.ScaleX > 0 ? 30.0f : -30.0f);
                float startY = t.Y;
                float speedX = t.ScaleX > 0 ? 15.0f : -15.0f;

                const sf::Texture &tex = _assets.GetTexture("BulletTex");
                float autoRadius = tex.getSize().x / 2.0f;

                _world.GetStorage<TransformComponent>().Add(bullet, {startX, startY, 1.0f, 1.0f});
                _world.GetStorage<VelocityComponent>().Add(bullet, {speedX, 0.0f});
                _world.GetStorage<SpriteComponent>().Add(bullet, {"BulletTex"});
                _world.GetStorage<ProjectileComponent>().Add(bullet, {});
                _world.GetStorage<ColliderComponent>().Add(bullet, {
                                                               ColliderType::Circle, autoRadius, {0.0f, 0.0f},
                                                               Projectile,
                                                               static_cast<uint16_t>(Tile)
                                                           });

                player.ShootCooldown = player.MaxCooldown;
                player.ShootAnimTimer = 15;
            }
        }
    }
};

#endif
