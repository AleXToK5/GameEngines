#ifndef ASTEROIDSPAWNERSYSTEM_H
#define ASTEROIDSPAWNERSYSTEM_H

#include <random>
#include "../../Ecs/Systems/ISystem.h"
#include "../../ConfigReader.h"

#include "../Components/PositionComponent.h"
#include "../Components/MovementComponent.h"
#include "../Components/RenderComponent.h"
#include "../Components/ColliderComponent.h"
#include "../Components/AsteroidComponent.h"

class AsteroidSpawnerSystem final : public ISystem {
    ConfigReader &_config;

    ComponentStorage<PositionComponent> &_positions;
    ComponentStorage<MovementComponent> &_movements;
    ComponentStorage<RenderComponent> &_renders;
    ComponentStorage<ColliderComponent> &_colliders;
    ComponentStorage<AsteroidComponent> &_asteroids;

    // Таймер спавна
    int _framesUntilNextSpawn;

    // Генератор случайных чисел
    std::mt19937 _rng;

    // Вспомогательные функции для случайных чисел
    float GetRandomFloat(float min, float max);

    int GetRandomInt(int min, int max);

    void ResetTimer();

public:
    AsteroidSpawnerSystem(World &world, ConfigReader &config);

    void OnInit() override;

    void OnUpdate() override;
};

#endif //ASTEROIDSPAWNERSYSTEM_H
