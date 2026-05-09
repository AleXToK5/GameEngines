#ifndef ANIMATIONSYSTEM_H
#define ANIMATIONSYSTEM_H

#include "../../Ecs/Systems/ISystem.h"
#include "../../Ecs/Filter/FilterBuilder.h"
#include "AnimatorComponent.h"
#include "../../GameEngine/Assets/AssetManager.h"

class AnimationSystem final : public ISystem {
    ComponentStorage<AnimatorComponent> &_animators;
    const AssetManager &_assets;
    Filter _filter;

public:
    AnimationSystem(World &world, const AssetManager &assets)
        : ISystem(world),
          _animators(world.GetStorage<AnimatorComponent>()),
          _assets(assets),
          _filter(FilterBuilder(world).With<AnimatorComponent>().Build()) {
    }

    void OnInit() override {
    }

    void OnUpdate() override {
        for (int e: _filter) {
            auto &anim = _animators.Get(e);
            const Animation &animData = _assets.GetAnimation(anim.CurrentAnimation);

            anim.FrameTimer++;
            if (anim.FrameTimer >= animData.FrameDuration()) {
                anim.FrameTimer = 0;
                anim.CurrentFrame = (anim.CurrentFrame + 1) % animData.FrameCount();
            }
        }
    }
};

#endif
