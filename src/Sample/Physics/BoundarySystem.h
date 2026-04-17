#ifndef BOUNDARYSYSTEM_H
#define BOUNDARYSYSTEM_H

#include "../../Ecs/Systems/ISystem.h"
#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"
#include "../../ConfigReader.h"
#include "PositionComponent.h"
#include <vector>

class BoundarySystem final : public ISystem {
    ConfigReader& _config;
    ComponentStorage<PositionComponent>& _positions;
    Filter _movables;

public:
    BoundarySystem(World &world, ConfigReader& config)
        : ISystem(world),
          _config(config),
          _positions(world.GetStorage<PositionComponent>()),
          _movables(FilterBuilder(world).With<PositionComponent>().Build())
    {
    }

    void OnInit() override {}

    void OnUpdate() override {
        float width = _config.GetWindowWidth();
        float height = _config.GetWindowHeight();

        // Собираем сущности на удаление в отдельный массив
        std::vector<int> toRemove;

        for (const auto ent : _movables) {
            auto& pos = _positions.Get(ent);

            // Если объект улетел на 100 пикселей за любую границу экрана
            if (pos.X < -100.f || pos.X > width + 100.f ||
                pos.Y < -100.f || pos.Y > height + 100.f)
            {
                toRemove.push_back(ent);
            }
        }

        // Удаляем собранные сущности из мира
        for (int ent : toRemove) {
            world.RemoveEntity(ent);
        }
    }
};

#endif //BOUNDARYSYSTEM_H