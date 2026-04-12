#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"
#include "../../Ecs/Systems/ISystem.h"

#include "../Components/MoveInputEvent.h"
#include "../Components/PositionComponent.h"
#include "../Components/MovementComponent.h"

class MovementSystem final : public ISystem {
    ComponentStorage<PositionComponent>& _positionComponents;
    ComponentStorage<MovementComponent>& _movementComponents;
    ComponentStorage<MoveInputEvent>& _eventComponents;

    Filter _moveables;
    Filter _moveInputEvents;

    void Print(int ent);

public:
    MovementSystem(World &world)
        : ISystem(world),
          _positionComponents(world.GetStorage<PositionComponent>()),
          _movementComponents(world.GetStorage<MovementComponent>()),
          _eventComponents(world.GetStorage<MoveInputEvent>()),
          _moveables(FilterBuilder(world)
              .With<PositionComponent>()
              .With<MovementComponent>()
              .Build()),
          _moveInputEvents(FilterBuilder(world)
              .With<MoveInputEvent>()
              .Build())
    {
    }

    void OnInit() override;
    void OnUpdate() override;
};

#endif //MOVEMENTSYSTEM_H