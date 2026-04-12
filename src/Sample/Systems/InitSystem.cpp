#include "InitSystem.h"

#include "../Components/PositionComponent.h"
#include "../Components/MovementComponent.h"
#include "../Components/RenderComponent.h"

void InitSystem::OnInit()
{
    const int player = world.CreateEntity();
    auto& positionsStorage = world.GetStorage<PositionComponent>();
    auto& movementsStorage = world.GetStorage<MovementComponent>();
    auto& renderStorage = world.GetStorage<RenderComponent>();

    positionsStorage.Add(player, PositionComponent{400.f, 300.f});
    movementsStorage.Add(player, MovementComponent{50.f, sf::Vector2f(0.f, 0.f)});
    renderStorage.Add(player, RenderComponent{20.f, sf::Color::Green});

    // Создаём ещё несколько тестовых сущностей
    for (int i = 0; i < 3; ++i) {
        const int entity = world.CreateEntity();
        positionsStorage.Add(entity, PositionComponent{200.f + i * 100.f, 150.f + i * 50.f});
        movementsStorage.Add(entity, MovementComponent{30.f, sf::Vector2f(0.f, 0.f)});
        renderStorage.Add(entity, RenderComponent{15.f, sf::Color::Cyan});
    }
}