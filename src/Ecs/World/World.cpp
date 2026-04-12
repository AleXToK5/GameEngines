#include "World.h"

World::World()
{
    _entities.reserve(DefaultEntitiesCapacity);
    _freeEntities.reserve(DefaultEntitiesCapacity);
    _componentStorages.reserve(16);
}

int World::CreateEntity()
{
    int entityId;
    if (!_freeEntities.empty())
    {
        entityId = _freeEntities.back();
        _freeEntities.pop_back();
        _entities[entityId].Recycle();
    }
    else
    {
        entityId = static_cast<int>(_entities.size());
        _entities.emplace_back(entityId, 1);
    }
    return entityId;
}

void World::RemoveEntity(const int e)
{
    auto& entity = _entities[e];
    if (entity.IsRemoved())
        return;

    const auto& components = entity.Components();
    if (!components.empty())
    {
        // Удаляем из всех хранилищ компонентов — они сами вызовут EntityComponentsChanged,
        // который в итоге вызовет Remove() и добавит в _freeEntities когда компонентов не останется
        for (int i = static_cast<int>(components.size()) - 1; i >= 0; i--)
            _componentStorages[components[i]]->Remove(e);
    }
    else
    {
        entity.Remove();
        _freeEntities.push_back(entity.Id);
    }
}

EntityId World::GetPackedEntity(const int e) const
{
    return _entities[e];
}

bool World::UnpackEntity(const EntityId &eId, int &e) const
{
    e = eId.Id;
    return IsEntityAlive(e) && eId.Gen() == _entities[e].Gen();
}

bool World::IsEntityAlive(const int e) const
{
    if (e < 0 || e >= static_cast<int>(_entities.size()))
        return false;
    const auto& entity = _entities[e];
    return !entity.IsRemoved() && entity.Id >= 0 && entity.Id < static_cast<int>(_entities.size()) && e == entity.Id;
}

void World::EntityComponentsChanged(const int e, const int storageId, const bool added)
{
    auto& entity = _entities[e];
    if (added)
    {
        entity.AddComponent(storageId);
    }
    else
    {
        const int newComponentsCount = entity.RemoveComponent(storageId);
        if (newComponentsCount == 0)
            RemoveEntity(e);
    }
}