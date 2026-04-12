#ifndef WORLD_H
#define WORLD_H

#include <unordered_map>
#include <vector>
#include <memory>

#include "IWorldInternal.h"
#include "../Entity/EntityId.h"
#include "../Components/ComponentStorage.hpp"

class World final : public internal::IWorldInternal {
    const int DefaultEntitiesCapacity = 64;

    std::vector<EntityId> _entities;
    std::vector<int> _freeEntities;

    std::unordered_map<size_t, std::shared_ptr<BaseComponentStorage>> _componentStoragesHash;
    std::vector<std::shared_ptr<BaseComponentStorage>> _componentStorages;

    int _storagesCount = 0;

public:
    World();

    int CreateEntity();
    void RemoveEntity(int e);

    EntityId GetPackedEntity(int e) const;
    bool UnpackEntity(const EntityId& eId, int& e) const;

    bool IsEntityAlive(int e) const override;
    void EntityComponentsChanged(int e, int storageId, bool added) override;

    template <typename T>
    std::shared_ptr<ComponentStorage<T>> GetRawStorage()
    {
        const auto typeHash = typeid(T).hash_code();
        const auto it = _componentStoragesHash.find(typeHash);
        if (it != _componentStoragesHash.end())
            return std::static_pointer_cast<ComponentStorage<T>>(it->second);

        // Хранилища ещё нет — создаём
        const int storageId = _storagesCount++;
        auto storage = std::make_shared<ComponentStorage<T>>(*this, storageId);
        _componentStoragesHash.insert({typeHash, storage});

        if (static_cast<int>(_componentStorages.size()) == static_cast<int>(_componentStorages.capacity()))
        {
            const int newCapacity = static_cast<int>(_componentStorages.capacity()) << 1;
            _componentStorages.reserve(newCapacity > 0 ? newCapacity : 16);
        }
        _componentStorages.push_back(storage);

        return storage;
    }

    template <typename T>
    ComponentStorage<T>& GetStorage()
    {
        return *GetRawStorage<T>();
    }
};

#endif //WORLD_H