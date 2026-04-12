#ifndef ENTITYID_H
#define ENTITYID_H

#include <iostream>
#include <vector>

class EntityId {
    std::vector<int> _components;
    int _gen;

public:
    const int Id;

    EntityId(const int id, const int gen);

    void AddComponent(int component);
    int RemoveComponent(int component);

    bool IsRemoved() const;
    const std::vector<int>& Components() const;
    int Gen() const;

    void Remove();
    void Recycle();

    bool Equals(const EntityId &other) const;

    bool operator ==(const EntityId& other) const;
    bool operator !=(const EntityId& other) const;

    friend std::ostream& operator<<(std::ostream& os, const EntityId& eId);
};

static const EntityId InvalidEntity(-1, -1);

#endif //ENTITYID_H