#ifndef INITSYSTEM_H
#define INITSYSTEM_H

#include "../../Ecs/Systems/IInitializer.h"

class InitSystem final : public IInitializer {
public:
    InitSystem(World &world) : IInitializer(world) { }

    void OnInit() override;
};



#endif //INITSYSTEM_H