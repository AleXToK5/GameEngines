#ifndef UISYSTEM_H
#define UISYSTEM_H

#include "../../Ecs/Systems/ISystem.h"
#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"
#include "../../Ecs/World/World.h"
#include "imgui.h"

#include "../Physics/TransformComponent.h"
#include "../Graphics/RenderSettingsComponent.h"

class UiSystem final : public ISystem {
    ComponentStorage<TransformComponent> &_transforms;
    ComponentStorage<RenderSettingsComponent> &_settings;

    Filter _allWithTransform;
    Filter _settingsFilter;

public:
    UiSystem(World &world);

    void OnInit() override;

    void OnUpdate() override;
};

#endif //UISYSTEM_H
