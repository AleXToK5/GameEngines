#include "EntityId.h"

EntityId::EntityId(const int id, const int gen) : _gen(gen), Id(id)
{
    _components.reserve(6);
}

void EntityId::AddComponent(const int component)
{
    _components.push_back(component);
}

int EntityId::RemoveComponent(const int component)
{
    for (int i = 0; i < static_cast<int>(_components.size()); i++)
    {
        if (_components[i] == component && i < static_cast<int>(_components.size()) - 1)
            _components[i] = _components[_components.size() - 1];
    }
    _components.pop_back();
    return static_cast<int>(_components.size());
}

bool EntityId::IsRemoved() const
{
    return _gen < 0;
}

const std::vector<int>& EntityId::Components() const
{
    return _components;
}

int EntityId::Gen() const
{
    return _gen;
}

void EntityId::Remove()
{
    // Инвалидируем поколение — умножаем на -1 (со сдвигом, чтобы 0 тоже стал отрицательным)
    _gen = -(_gen + 1);
    _components.clear();
}

void EntityId::Recycle()
{
    // Восстанавливаем валидное поколение
    _gen *= -1;
}

bool EntityId::Equals(const EntityId &other) const
{
    return Id == other.Id && _gen == other._gen;
}

bool EntityId::operator==(const EntityId &other) const
{
    return this->Equals(other);
}

bool EntityId::operator!=(const EntityId &other) const
{
    return !this->Equals(other);
}

std::ostream& operator<<(std::ostream &os, const EntityId &eId)
{
    os << "EntityId(id=" << eId.Id << ", gen=" << eId._gen << ")";
    return os;
}