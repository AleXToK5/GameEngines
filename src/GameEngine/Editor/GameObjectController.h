#pragma once
#include <string>
#include <cmath>
#include "GameObjectFactory.h"
#include "RenderState.h"
#include "CameraService.h"
#include "../../Ecs/World/World.h"
#include "../../Sample/Physics/TransformComponent.h"
#include "../../Sample/Graphics/SpriteComponent.h"

class GameObjectController {
    World& _world;
    float _windowHeight;

public:
    GameObjectController(World& world, float windowHeight)
        : _world(world), _windowHeight(windowHeight) {}

    // Подбирает объект под курсором для перетаскивания
    bool TryPickUp(RenderState& state,
                   const sf::RenderWindow& window,
                   const sf::Vector2i& mousePixel) {
        sf::Vector2f worldPos = CameraService::ScreenToWorld(window, mousePixel, state);

        auto& transforms = _world.GetStorage<TransformComponent>();
        float best = EditorConstants::TileSize * 0.75f;
        int picked = -1;

        for (int id = 0; id < _world.EntityCount(); id++) {
            if (!_world.IsEntityAlive(id)) continue;
            if (!transforms.Has(id)) continue;

            const auto& t = transforms.Get(id);
            float dx = t.X - worldPos.x;
            float dy = t.Y - worldPos.y;
            if (std::sqrt(dx * dx + dy * dy) < best) {
                best = std::sqrt(dx * dx + dy * dy);
                picked = id;
            }
        }

        if (picked != -1) {
            const auto& t = transforms.Get(picked);
            state.HeldEntity = picked;
            state.HeldOffset = {t.X - worldPos.x, t.Y - worldPos.y};
            return true;
        }
        return false;
    }

    // Перемещает удерживаемый объект к позиции мыши
    void MoveHeld(RenderState& state,
                  const sf::RenderWindow& window,
                  const sf::Vector2i& mousePixel) {
        if (state.HeldEntity == -1) return;
        if (!_world.IsEntityAlive(state.HeldEntity)) {
            state.HeldEntity = -1;
            return;
        }

        sf::Vector2f worldPos = CameraService::ScreenToWorld(window, mousePixel, state);
        worldPos += state.HeldOffset;
        worldPos = SnapToGrid(worldPos);

        auto& t = _world.GetStorage<TransformComponent>().Get(state.HeldEntity);
        t.X = worldPos.x;
        t.Y = worldPos.y;
    }

    void Drop(RenderState& state) {
        state.HeldEntity = -1;
    }

    // Удаляет объект под курсором
    void TryDelete(const sf::RenderWindow& window,
                   const sf::Vector2i& mousePixel,
                   const RenderState& state) {
        sf::Vector2f worldPos = CameraService::ScreenToWorld(window, mousePixel, state);
        auto& transforms = _world.GetStorage<TransformComponent>();

        float best = EditorConstants::TileSize * 0.75f;
        int toDelete = -1;

        for (int id = 0; id < _world.EntityCount(); id++) {
            if (!_world.IsEntityAlive(id)) continue;
            if (!transforms.Has(id)) continue;

            const auto& t = transforms.Get(id);
            float dx = t.X - worldPos.x;
            float dy = t.Y - worldPos.y;
            float dist = std::sqrt(dx * dx + dy * dy);
            if (dist < best) {
                best = dist;
                toDelete = id;
            }
        }

        if (toDelete != -1)
            _world.RemoveEntity(toDelete);
    }

private:
    static sf::Vector2f SnapToGrid(sf::Vector2f pos) {
        float ts = EditorConstants::TileSize;
        return {
            std::floor(pos.x / ts) * ts + ts / 2.f,
            std::floor(pos.y / ts) * ts + ts / 2.f
        };
    }
};