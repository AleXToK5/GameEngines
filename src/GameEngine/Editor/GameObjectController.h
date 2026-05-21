#pragma once
#include <string>
#include <cmath>
#include <unordered_map>
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

    bool TryPickUp(RenderState& state,
                   const sf::RenderWindow& window,
                   const sf::Vector2i& mousePixel,
                   bool ctrlHeld) {
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
            float dist = std::sqrt(dx * dx + dy * dy);
            if (dist < best) { best = dist; picked = id; }
        }

        if (picked == -1) {
            if (!ctrlHeld) state.Selection.clear();
            return false;
        }

        if (ctrlHeld) {
            if (state.Selection.count(picked))
                state.Selection.erase(picked);
            else
                state.Selection.insert(picked);
            return false;
        }

        if (!state.Selection.count(picked))
            state.Selection.clear();

        state.Selection.insert(picked);
        state.HeldEntity = picked;

        state.SelectionOffsets.clear();
        for (int sel : state.Selection) {
            if (!_world.IsEntityAlive(sel) || !transforms.Has(sel)) continue;
            const auto& t = transforms.Get(sel);
            state.SelectionOffsets[sel] = {t.X - worldPos.x, t.Y - worldPos.y};
        }

        return true;
    }

    void BeginDragNew(RenderState& state, int entityId,
                      const sf::RenderWindow& window,
                      const sf::Vector2i& mousePixel) {
        state.Selection.clear();
        state.Selection.insert(entityId);
        state.HeldEntity = entityId;

        sf::Vector2f worldPos = CameraService::ScreenToWorld(window, mousePixel, state);
        state.SelectionOffsets.clear();
        state.SelectionOffsets[entityId] = {0.f, 0.f};

        auto& t = _world.GetStorage<TransformComponent>().Get(entityId);
        sf::Vector2f snapped = SnapToGrid(worldPos);
        t.X = snapped.x;
        t.Y = snapped.y;
    }

    void MoveHeld(RenderState& state,
                  const sf::RenderWindow& window,
                  const sf::Vector2i& mousePixel) {
        if (state.HeldEntity == -1) return;

        sf::Vector2f worldPos = CameraService::ScreenToWorld(window, mousePixel, state);
        auto& transforms = _world.GetStorage<TransformComponent>();

        for (auto& [id, offset] : state.SelectionOffsets) {
            if (!_world.IsEntityAlive(id) || !transforms.Has(id)) continue;
            sf::Vector2f newPos = SnapToGrid({worldPos.x + offset.x, worldPos.y + offset.y});
            auto& t = transforms.Get(id);
            t.X = newPos.x;
            t.Y = newPos.y;
        }
    }

    void Drop(RenderState& state) {
        state.HeldEntity = -1;
        state.SelectionOffsets.clear();
        state.SelectedObject = "";
    }

    void TryDelete(const sf::RenderWindow& window,
                   const sf::Vector2i& mousePixel,
                   RenderState& state) {
        if (!state.Selection.empty()) {
            for (int id : state.Selection) {
                if (_world.IsEntityAlive(id))
                    _world.RemoveEntity(id);
            }
            state.Selection.clear();
            return;
        }

        sf::Vector2f worldPos = CameraService::ScreenToWorld(window, mousePixel, state);
        auto& transforms = _world.GetStorage<TransformComponent>();
        float best = EditorConstants::TileSize * 0.75f;
        int toDelete = -1;

        for (int id = 0; id < _world.EntityCount(); id++) {
            if (!_world.IsEntityAlive(id) || !transforms.Has(id)) continue;
            const auto& t = transforms.Get(id);
            float dx = t.X - worldPos.x;
            float dy = t.Y - worldPos.y;
            float dist = std::sqrt(dx * dx + dy * dy);
            if (dist < best) { best = dist; toDelete = id; }
        }

        if (toDelete != -1) _world.RemoveEntity(toDelete);
    }

    void Copy(RenderState& state) {
        if (state.Selection.empty()) return;

        auto& transforms = _world.GetStorage<TransformComponent>();
        auto& sprites    = _world.GetStorage<SpriteComponent>();

        sf::Vector2f center{0.f, 0.f};
        int count = 0;
        for (int id : state.Selection) {
            if (!_world.IsEntityAlive(id) || !transforms.Has(id)) continue;
            const auto& t = transforms.Get(id);
            center.x += t.X;
            center.y += t.Y;
            count++;
        }
        if (count == 0) return;
        center /= static_cast<float>(count);

        state.Clipboard.clear();
        for (int id : state.Selection) {
            if (!_world.IsEntityAlive(id) || !transforms.Has(id)) continue;
            const auto& t = transforms.Get(id);
            std::string texName = sprites.Has(id) ? sprites.Get(id).TextureName : "";
            std::string objName = EditorConstants::TextureToObjectName(texName);
            if (objName.empty()) continue;

            state.Clipboard.push_back({
                texName,
                objName,
                {t.X - center.x, t.Y - center.y}
            });
        }
    }

    void Paste(RenderState& state,
               const sf::RenderWindow& window,
               const sf::Vector2i& mousePixel) {
        if (state.Clipboard.empty()) return;

        sf::Vector2f worldPos = CameraService::ScreenToWorld(window, mousePixel, state);
        sf::Vector2f pasteCenter = SnapToGrid(worldPos);

        state.Selection.clear();

        for (const auto& entry : state.Clipboard) {
            sf::Vector2f pos = SnapToGrid({
                pasteCenter.x + entry.RelPos.x,
                pasteCenter.y + entry.RelPos.y
            });
            int newEnt = GameObjectFactory::Create(_world, entry.ObjectName, pos.x, pos.y);
            if (newEnt != -1)
                state.Selection.insert(newEnt);
        }
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