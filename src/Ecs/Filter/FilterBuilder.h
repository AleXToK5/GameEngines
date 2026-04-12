#ifndef FILTERBUILDER_H
#define FILTERBUILDER_H

#include "Filter.h"
#include "../World/World.h"

class FilterBuilder
{
    World& _world;
    std::vector<std::shared_ptr<BaseComponentStorage>> _componentStorages;

public:
    FilterBuilder(World& world);

    template <typename T>
    FilterBuilder& With()
    {
        auto storage = _world.GetRawStorage<T>();
        _componentStorages.push_back(storage);
        return *this;
    }

    Filter Build() const;
};

#endif //FILTERBUILDER_H