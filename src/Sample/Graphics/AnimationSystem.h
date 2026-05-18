#ifndef ANIMATIONSYSTEM_H
#define ANIMATIONSYSTEM_H

#include <vector>
#include "../../Ecs/Systems/ISystem.h"
#include "../../Ecs/Filter/FilterBuilder.h"
#include "AnimatorComponent.h"
#include "DestroyAfterAnimationComponent.h"
#include "../../GameEngine/Assets/AssetManager.h"

class AnimationSystem final : public ISystem {
    World &_world;
    ComponentStorage<AnimatorComponent> &_animators;
    const AssetManager &_assets;
    Filter _filter;

public:
    AnimationSystem(World &world, const AssetManager &assets)
        : ISystem(world),
          _world(world),
          _animators(world.GetStorage<AnimatorComponent>()),
          _assets(assets),
          _filter(FilterBuilder(world).With<AnimatorComponent>().Build()) {
    }

    void OnInit() override {
    }

    void OnUpdate() override {
        std::vector<int> entitiesToRemove;

        for (int e: _filter) {
            try {
                auto &anim = _animators.Get(e);
                const Animation &animData = _assets.GetAnimation(anim.CurrentAnimation);

                anim.FrameTimer++;
                if (anim.FrameTimer >= animData.FrameDuration()) {
                    anim.FrameTimer = 0;

                    if (anim.CurrentFrame + 1 >= animData.FrameCount()) {
                        if (_world.GetStorage<DestroyAfterAnimationComponent>().Has(e)) {
                            entitiesToRemove.push_back(e);
                        } else {
                            anim.CurrentFrame = 0;
                        }
                    } else {
                        anim.CurrentFrame++;
                    }
                }
            }
            catch (const std::exception& ex) {
                std::cerr << "[AnimationSystem] Entity " << e
                          << " error: " << ex.what() << std::endl;
            }
        }

        for (int e: entitiesToRemove) {
            _world.RemoveEntity(e);
        }
    }
};

#endif
