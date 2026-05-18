#pragma once
#include <typeindex>
#include "SerializedEntity.h"

class IComponentSerializer {
public:
    virtual ~IComponentSerializer() = default;

    virtual bool Serialize(int entity, std::type_index type, SerializedComponent& out) const = 0;
    virtual bool Deserialize(int entity, const SerializedComponent& data) = 0;
    virtual const std::unordered_map<size_t, std::type_index>& HashToTypeIdxMap() const = 0;
};