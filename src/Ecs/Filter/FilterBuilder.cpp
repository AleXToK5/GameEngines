#include "FilterBuilder.h"

FilterBuilder::FilterBuilder(World& world) : _world(world)
{
    _componentStorages.reserve(4);
}

Filter FilterBuilder::Build() const
{
    return Filter(_world, _componentStorages);
}