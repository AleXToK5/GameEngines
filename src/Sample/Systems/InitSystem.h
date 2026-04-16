#ifndef INITSYSTEM_H
#define INITSYSTEM_H

#include "../../Ecs/Systems/IInitializer.h"
#include "../../ConfigReader.h"

class InitSystem final : public IInitializer {
    ConfigReader &_config;

public:
    InitSystem(World &world, ConfigReader &config)
        : IInitializer(world), _config(config) {
    }

    void OnInit() override;
};

#endif //INITSYSTEM_H
