#ifndef GOOMBACOMPONENT_H
#define GOOMBACOMPONENT_H

enum class GoombaState { Patrol, Pursue };

struct GoombaComponent {
    GoombaState State = GoombaState::Patrol;
    float PatrolLeftX = 0.f;
    float PatrolRightX = 0.f;
    float SpeedX = 3.0f;
    int Direction = -1; // -1 - идет влево, 1 - идет вправо
};

#endif // GOOMBACOMPONENT_H
