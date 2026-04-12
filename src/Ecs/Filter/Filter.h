#ifndef FILTER_H
#define FILTER_H

#include <memory>
#include <utility>
#include <vector>
#include <span>

#include "../Components/BaseComponentStorage.h"
#include "../World/World.h"

class Filter {
    World& _world;
    std::vector<std::shared_ptr<BaseComponentStorage>> _componentStorages;
    size_t _minStorageIndex = -1;

    size_t FindMinComponentStorage() const;

public:
    Filter(World& world,
        const std::vector<std::shared_ptr<BaseComponentStorage>>& componentStorages);

    class Iterator
    {
        World& _world;
        std::span<const int> _minStorageEntities;  // без const& — span копируется
        const std::vector<std::shared_ptr<BaseComponentStorage>>& _storages;
        const size_t _minStorageIndex;
        int _currentEntity;
        size_t _current;

        bool HasAllComponents() const;
        Iterator& Increment();

    public:
        using iterator_category = std::input_iterator_tag;
        using value_type = int;
        using difference_type = std::ptrdiff_t;

        // Конструктор: ищет первую сущность из minStorageEntities,
        // которая присутствует во всех остальных хранилищах из storages.
        Iterator(World& world,
            const std::vector<std::shared_ptr<BaseComponentStorage>>& storages,
            std::span<const int> minStorageEntities,  // без const&
            size_t minStorageIndex,
            size_t current);

        value_type operator*() const;

        Iterator& operator++();

        bool operator!=(const Iterator& other) const;
    };

    Iterator begin();

    Iterator end();
};

#endif //FILTER_H