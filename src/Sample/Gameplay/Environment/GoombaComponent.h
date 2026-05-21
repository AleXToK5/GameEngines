#ifndef GOOMBACOMPONENT_H
#define GOOMBACOMPONENT_H

enum class GoombaState { Patrol, Idle, Pursue };

struct GoombaComponent {
    GoombaState State = GoombaState::Patrol;
    float PatrolLeftX = 0.f;
    float PatrolRightX = 0.f;
    float SpeedX = 2.0f;
    int Direction = -1;
    float WaitTimer = 0.f;
};

#endif // GOOMBACOMPONENT_H
