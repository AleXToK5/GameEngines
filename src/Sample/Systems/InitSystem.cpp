#include "InitSystem.h"

#include "../Components/PositionComponent.h"
#include "../Components/MovementComponent.h"
#include "../Components/RenderComponent.h"
#include "../Components/PlayerComponent.h"
#include "../Components/ColliderComponent.h"
#include "../Components/GameStateComponent.h"
#include "../Components/SpawnerConfigComponent.h"

void InitSystem::OnInit() {
    const int player = world.CreateEntity();
    int gameStateEntity = world.CreateEntity();

    auto &positionsStorage = world.GetStorage<PositionComponent>();
    auto &movementsStorage = world.GetStorage<MovementComponent>();
    auto &renderStorage = world.GetStorage<RenderComponent>();
    auto &playerStorage = world.GetStorage<PlayerComponent>();
    auto &colliderStorage = world.GetStorage<ColliderComponent>();
    auto &gameStateStorage = world.GetStorage<GameStateComponent>();
    auto &spawnerConfigStorage = world.GetStorage<SpawnerConfigComponent>();

    // Ставим игрока по центру внизу экрана
    float startX = _config.GetWindowWidth() / 2.0f;
    float startY = _config.GetWindowHeight() - 50.0f;

    positionsStorage.Add(player, PositionComponent{startX, startY});

    // Скорость берём из конфига
    movementsStorage.Add(player, MovementComponent{_config.GetPlayerMovementSpeed() / 60.0f, sf::Vector2f(0.f, 0.f)});

    // Треугольник
    renderStorage.Add(player, RenderComponent{ShapeType::Polygon, 20.f, 3, {0.f, 0.f}, sf::Color::Green});
    colliderStorage.Add(player, ColliderComponent{ColliderType::Circle, 20.f, {0.f, 0.f}, Player, Asteroid});
    gameStateStorage.Add(gameStateEntity, GameStateComponent{0, false});

    // Переводим миллисекунды кулдауна в кадры (при 60 FPS: 1000мс = 60 кадров)
    int cooldown = (_config.GetPlayerShootCooldownMs() * 60) / 1000;
    playerStorage.Add(player, PlayerComponent{0, cooldown});

    spawnerConfigStorage.Add(gameStateEntity, SpawnerConfigComponent{
                                 _config.GetAsteroidSpawnTimeMinMs(),
                                 _config.GetAsteroidSpawnTimeMaxMs(),
                                 false
                             });
}
