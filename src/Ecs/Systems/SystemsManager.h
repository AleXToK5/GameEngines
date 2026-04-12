#ifndef SYSTEMSMANAGER_H
#define SYSTEMSMANAGER_H

#include <memory>
#include <vector>

#include "ISystem.h"
#include "../World/World.h"

class SystemsManager
{
    std::vector<std::shared_ptr<IInitializer>> _newInitializers;
    std::vector<std::shared_ptr<IInitializer>> _initializers;
    std::vector<std::shared_ptr<ISystem>> _systems;

    World& _world;
public:

    SystemsManager(World& world);

    SystemsManager& AddInitializer(std::shared_ptr<IInitializer> initializer);
    SystemsManager& AddSystem(std::shared_ptr<ISystem> system);

    void Initialize();
    void Update();
};



#endif //SYSTEMSMANAGER_H