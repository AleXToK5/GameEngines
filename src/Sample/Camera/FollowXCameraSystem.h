#ifndef FOLLOWXCAMERASYSTEM_H
#define FOLLOWXCAMERASYSTEM_H

#include <SFML/Graphics/RenderWindow.hpp>
#include "../../Ecs/Systems/ISystem.h"
#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"
#include "CameraComponent.h"
#include "FollowXCameraTag.h"
#include "../Physics/TransformComponent.h"
#include "../Gameplay/PlayerTag.h"

class FollowXCameraSystem final : public ISystem {
    sf::RenderWindow &_window;
    ComponentStorage<CameraComponent> &_cameras;
    ComponentStorage<TransformComponent> &_transforms;
    Filter _cameraFilter;
    Filter _playerFilter;

public:
    FollowXCameraSystem(World &world, sf::RenderWindow &window)
        : ISystem(world),
          _window(window),
          _cameras(world.GetStorage<CameraComponent>()),
          _transforms(world.GetStorage<TransformComponent>()),
          _cameraFilter(FilterBuilder(world)
              .With<CameraComponent>()
              .With<FollowXCameraTag>()
              .Build()),
          _playerFilter(FilterBuilder(world)
              .With<TransformComponent>()
              .With<PlayerTag>()
              .Build()) {
    }

    void OnInit() override {
        int e = world.CreateEntity();
        float hw = static_cast<float>(_window.getSize().x) / 2.f;
        float hh = static_cast<float>(_window.getSize().y) / 2.f;
        sf::View view(sf::Vector2f(hw, hh),
                      sf::Vector2f(static_cast<float>(_window.getSize().x),
                                   static_cast<float>(_window.getSize().y)));
        world.GetStorage<CameraComponent>().Add(e, {view});
        world.GetStorage<FollowXCameraTag>().Add(e, {});
    }

    void OnUpdate() override {
        // Находим позицию игрока
        float playerX = -1.f;
        for (int e: _playerFilter) {
            playerX = _transforms.Get(e).X;
            break;
        }

        for (int e: _cameraFilter) {
            auto &cam = _cameras.Get(e);
            float hw = static_cast<float>(_window.getSize().x) / 2.f;
            float hh = static_cast<float>(_window.getSize().y) / 2.f;

            // Следим по X, фиксируем Y по центру экрана
            // Не уходим левее начала уровня
            float camX = (playerX >= 0.f) ? std::max(playerX, hw) : hw;
            cam.View.setCenter({camX, hh});
            _window.setView(cam.View);
            break;
        }
    }
};

#endif //FOLLOWXCAMERASYSTEM_H
