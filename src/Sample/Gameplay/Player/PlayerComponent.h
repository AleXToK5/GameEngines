#ifndef PLAYERCOMPONENT_H
#define PLAYERCOMPONENT_H

struct PlayerComponent {
    float SpeedX = 0.f;
    float JumpSpeedY = 0.f;
    float MaxSpeed = 0.f;
    bool IsGrounded = false;
    int ShootCooldown = 0;
    int MaxCooldown = 20;
    bool IsFinished = false;
    float SpawnX = 0.f;
    float SpawnY = 0.f;
    int ShootAnimTimer = 0;
};

#endif
