#ifndef ISYSTEM_H
#define ISYSTEM_H

#include "IInitializer.h"

class ISystem : public IInitializer
{
public:
    ISystem(World& world) : IInitializer(world) {}

    ~ISystem() override = default;
    virtual void OnUpdate() = 0;
};

#endif //ISYSTEM_H