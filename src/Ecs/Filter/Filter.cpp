#include "Filter.h"

#include <limits>


Filter::Filter(World& world,
    const std::vector<std::shared_ptr<BaseComponentStorage>>& componentStorages)
    : _world(world), _componentStorages(componentStorages)
{
}

size_t Filter::FindMinComponentStorage() const
{
    size_t minIndex = 0;
    size_t minCount = std::numeric_limits<size_t>::max();

    for (size_t i = 0; i < _componentStorages.size(); i++)
    {
        const size_t count = _componentStorages[i]->Count();
        if (count < minCount)
        {
            minCount = count;
            minIndex = i;
        }
    }

    return minIndex;
}


Filter::Iterator::Iterator(World& world,
    const std::vector<std::shared_ptr<BaseComponentStorage>>& storages,
    std::span<const int> minStorageEntities,
    size_t minStorageIndex,
    size_t current)
    : _world(world),
      _minStorageEntities(minStorageEntities),
      _storages(storages),
      _minStorageIndex(minStorageIndex),
      _currentEntity(-1),
      _current(current)
{
    // Ищем первую сущность из минимального хранилища, которая есть во всех остальных
    while (_current < _minStorageEntities.size())
    {
        _currentEntity = _minStorageEntities[_current];
        if (HasAllComponents())
            break;
        ++_current;
    }
}

bool Filter::Iterator::HasAllComponents() const
{
    for (size_t i = 0; i < _storages.size(); i++)
    {
        if (i == _minStorageIndex)
            continue;
        if (!_storages[i]->Has(_currentEntity))
            return false;
    }
    return true;
}

Filter::Iterator& Filter::Iterator::Increment()
{
    while (++_current < _minStorageEntities.size())
    {
        _currentEntity = _minStorageEntities[_current];
        if (HasAllComponents())
            break;
    }
    return *this;
}

Filter::Iterator::value_type Filter::Iterator::operator*() const
{
    return _currentEntity;
}

Filter::Iterator& Filter::Iterator::operator++()
{
    return Increment();
}

bool Filter::Iterator::operator!=(const Iterator &other) const
{
    return _current != other._current || &_world != &other._world;
}

Filter::Iterator Filter::begin()
{
    _minStorageIndex = FindMinComponentStorage();
    auto minStorageEntities = _componentStorages[_minStorageIndex]->Entities();
    return Iterator(_world, _componentStorages, minStorageEntities, _minStorageIndex, 0);
}

Filter::Iterator Filter::end()
{
    _minStorageIndex = FindMinComponentStorage();
    auto minStorageEntities = _componentStorages[_minStorageIndex]->Entities();
    return Iterator(_world, _componentStorages, minStorageEntities, _minStorageIndex,
        minStorageEntities.size());
}