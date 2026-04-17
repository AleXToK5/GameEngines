#ifndef PLAYERCOMPONENT_H
#define PLAYERCOMPONENT_H

struct PlayerComponent
{
    int ShootCooldown; // Текущий счетчик до следующего выстрела
    int MaxCooldown;   // Максимальное значение между выстрелами
};

#endif //PLAYERCOMPONENT_H