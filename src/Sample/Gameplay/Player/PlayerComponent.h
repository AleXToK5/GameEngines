#ifndef PLAYERCOMPONENT_H
#define PLAYERCOMPONENT_H

struct PlayerComponent {
    float SpeedX = 0.f;
    float JumpSpeedY = 0.f;
    float MaxSpeed = 0.f;
    bool IsGrounded = false;
};

#endif
