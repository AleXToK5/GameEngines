#ifndef ANIMATORCOMPONENT_H
#define ANIMATORCOMPONENT_H
#include <string>

struct AnimatorComponent {
    std::string CurrentAnimation = "IdleAnim";
    int CurrentFrame = 0;
    int FrameTimer = 0;
};

#endif //ANIMATORCOMPONENT_H
