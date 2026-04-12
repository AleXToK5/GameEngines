#ifndef COMPONENTSTORAGE_H
#define COMPONENTSTORAGE_H

#include <vector>

#include "BaseComponentStorage.h"
#include "../World/IWorldInternal.h"

template <typename T>
class ComponentStorage final : public BaseComponentStorage {
    internal::IWorldInternal& _world;

    std::vector<T> _data;      // Для плотного хранения данных
    std::vector<int> _sparse;  // Для разреженного хранения индексов элементов (сущностей)
    std::vector<int> _dense;   // Для плотного хранения реально существующих элементов (сущностей, на которых есть компоненты)
    
    int _count = 0;            // Текущее число элементов

    int _id;

    void Resize(const int sparseSize, const int dataSize)
    {
        int oldSparseSize = _sparse.size();
        if (oldSparseSize < sparseSize) {
            _sparse.resize(sparseSize);
            std::fill_n(_sparse.data() + oldSparseSize, 
                        sparseSize - oldSparseSize, -1);
        }
        
        int oldDataSize = _dense.size();
        if (oldDataSize < dataSize) {
            _dense.resize(dataSize);
            _data.resize(dataSize);
            std::fill_n(_dense.data() + oldDataSize, 
                        dataSize - oldDataSize, -1);
        }
    }

public:
    ComponentStorage(internal::IWorldInternal& w, const int id)
        : _world(w), _id(id)
    {
        Resize(64, 64);
    }

    bool Has(const int e) const override
    {
        return e < static_cast<int>(_sparse.size()) && _sparse[e] != -1;
    }

    T& Get(const int e)
    {
        return _data[_sparse[e]];
    }

    T& Add(const int e, const T& value)
    {
        Resize((e / 64 + 1) * 64, _data.size() == _count + 1 ? _data.size() + 64 : _data.size());
        
        _data[_count] = value;
        _dense[_count] = e;
        _sparse[e] = _count;
        _count++;

        return _data[_sparse[e]];
    }

    void Remove(const int e) override
    {
        int arrayIndex = _sparse[e];
        int lastEntityId = _dense[--_count];
        
        _data[arrayIndex] = _data[_count];
        _dense[arrayIndex] = lastEntityId;
        _sparse[lastEntityId] = arrayIndex;
        _sparse[e] = -1;
    }

    std::span<const T> All() const
    {
        return std::span(_data.begin(), _count);
    }

    std::span<const int> Entities() const override
    {
        return std::span(_dense.begin(), _count);
    }

    int Count() const override
    {
        return _count;
    }

    int Id() const override
    {
        return _id;
    }
};

#endif //COMPONENTSTORAGE_H