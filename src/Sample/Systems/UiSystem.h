#ifndef UISYSTEM_H
#define UISYSTEM_H

#include "../../Ecs/Systems/ISystem.h"
#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"
#include "../../Ecs/World/World.h"
#include "imgui.h"

#include "../Components/PositionComponent.h"
#include "../Components/SpawnerConfigComponent.h"

class UiSystem final : public ISystem {
    ComponentStorage<PositionComponent>& _positions;
    ComponentStorage<SpawnerConfigComponent>& _configs;
    
    Filter _allWithPosition;
    Filter _configFilter;

public:
    UiSystem(World &world);
    void OnInit() override {}
    void OnUpdate() override;
};

#endif