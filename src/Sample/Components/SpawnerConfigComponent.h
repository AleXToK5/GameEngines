#ifndef SPAWNERCONFIGCOMPONENT_H
#define SPAWNERCONFIGCOMPONENT_H

struct SpawnerConfigComponent {
    int MinSpawnTimeMs;
    int MaxSpawnTimeMs;
    bool ForceSpawn = false;
};

#endif //SPAWNERCONFIGCOMPONENT_H
