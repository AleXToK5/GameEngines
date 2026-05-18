#pragma once
#include <SFML/Graphics.hpp>
#include "../../../Ecs/Systems/ISystem.h"
#include "../../../GameEngine/Input/InputAction.h"
#include "../RenderState.h"

class EditorInputSystem final : public ISystem {
    RenderState& _state;

    std::shared_ptr<InputAction> _left;
    std::shared_ptr<InputAction> _right;
    std::shared_ptr<InputAction> _up;
    std::shared_ptr<InputAction> _down;

    static constexpr float MoveSpeed = 8.f;

public:
    EditorInputSystem(World& world,
                      RenderState& state,
                      std::shared_ptr<InputAction> left,
                      std::shared_ptr<InputAction> right,
                      std::shared_ptr<InputAction> up,
                      std::shared_ptr<InputAction> down)
        : ISystem(world),
          _state(state),
          _left(left), _right(right), _up(up), _down(down) {}

    void OnInit() override {}

    void OnUpdate() override {
        float speed = MoveSpeed * _state.Zoom;

        if (_left->Type()  == ActionType::Start) _state.CameraCenter.x -= speed;
        if (_right->Type() == ActionType::Start) _state.CameraCenter.x += speed;
        if (_up->Type()    == ActionType::Start) _state.CameraCenter.y -= speed;
        if (_down->Type()  == ActionType::Start) _state.CameraCenter.y += speed;
    }
};