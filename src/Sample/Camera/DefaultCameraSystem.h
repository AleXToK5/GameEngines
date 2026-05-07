#ifndef DEFAULTCAMERASYSTEM_H
#define DEFAULTCAMERASYSTEM_H

#include <SFML/Graphics/RenderWindow.hpp>
#include "../../Ecs/Systems/ISystem.h"
#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"
#include "CameraComponent.h"
#include "DefaultCameraTag.h"

class DefaultCameraSystem final : public ISystem {
    sf::RenderWindow &_window;
    ComponentStorage<CameraComponent> &_cameras;
    Filter _filter;

public:
    DefaultCameraSystem(World &world, sf::RenderWindow &window)
        : ISystem(world),
          _window(window),
          _cameras(world.GetStorage<CameraComponent>()),
          _filter(FilterBuilder(world)
              .With<CameraComponent>()
              .With<DefaultCameraTag>()
              .Build()) {
    }

    void OnInit() override {
        // Создаём сущность с камерой
        int e = world.CreateEntity();
        sf::View view(sf::FloatRect(
            {0.f, 0.f},
            {
                static_cast<float>(_window.getSize().x),
                static_cast<float>(_window.getSize().y)
            }));
        world.GetStorage<CameraComponent>().Add(e, {view});
        world.GetStorage<DefaultCameraTag>().Add(e, {});
    }

    void OnUpdate() override {
        for (int e: _filter) {
            _window.setView(_cameras.Get(e).View);
            break; // Только одна камера
        }
    }
};

#endif //DEFAULTCAMERASYSTEM_H
