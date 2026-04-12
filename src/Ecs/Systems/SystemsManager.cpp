#include "SystemsManager.h"

SystemsManager::SystemsManager(World& world) : _world(world)
{
}

SystemsManager& SystemsManager::AddInitializer(std::shared_ptr<IInitializer> initializer)
{
    _newInitializers.push_back(std::move(initializer));
    return *this;
}

SystemsManager& SystemsManager::AddSystem(std::shared_ptr<ISystem> system)
{
    // Система — это одновременно и инициализатор, и update-система
    _newInitializers.push_back(system);
    _systems.push_back(std::move(system));
    return *this;
}

void SystemsManager::Initialize()
{
    if (_newInitializers.empty())
        return;

    for (size_t i = 0; i < _newInitializers.size(); i++)
    {
        _newInitializers[i]->OnInit();
        _initializers.push_back(_newInitializers[i]);
    }
    _newInitializers.clear();
}

void SystemsManager::Update()
{
    // Сначала инициализируем новые системы, затем обновляем все
    Initialize();

    for (size_t i = 0; i < _systems.size(); i++)
    {
        _systems[i]->OnUpdate();
    }
}