#pragma once
#include <imgui.h>
#include <SFML/Graphics.hpp>
#include "../../../Ecs/Systems/ISystem.h"
#include "../../../GameEngine/Input/InputAction.h"
#include "../../../Sample/Physics/TransformComponent.h"
#include "../RenderState.h"
#include "../CameraService.h"
#include "../GameObjectController.h"
#include "../GameObjectFactory.h"

class EditorMouseInputSystem final : public ISystem {
    sf::RenderWindow& _window;
    RenderState& _state;
    GameObjectController& _controller;

    std::shared_ptr<InputAction> _lmb;
    std::shared_ptr<InputAction> _rmb;
    std::shared_ptr<InputAction> _mmb;
    std::shared_ptr<InputAction> _wheel;
    std::shared_ptr<InputAction> _move;

    bool _lmbWasDown  = false;
    bool _rmbWasDown  = false;
    bool _copyWasDown  = false;
    bool _pasteWasDown = false;

public:
    EditorMouseInputSystem(World& world,
                           sf::RenderWindow& window,
                           RenderState& state,
                           GameObjectController& controller,
                           std::shared_ptr<InputAction> lmb,
                           std::shared_ptr<InputAction> rmb,
                           std::shared_ptr<InputAction> mmb,
                           std::shared_ptr<InputAction> wheel,
                           std::shared_ptr<InputAction> move)
        : ISystem(world),
          _window(window), _state(state), _controller(controller),
          _lmb(lmb), _rmb(rmb), _mmb(mmb), _wheel(wheel), _move(move) {}

    void OnInit() override {}

    void OnUpdate() override {
        sf::Vector2i mousePixel = _move->Value2();
        ImGuiIO& io = ImGui::GetIO();

        if (_wheel->Type() == ActionType::Start) {
            CameraService::ApplyZoom(_state, static_cast<float>(_wheel->Value()));
            _wheel->Type() = ActionType::None;
        }

        if (_mmb->Type() == ActionType::Start && !_state.IsPanning)
            CameraService::BeginPan(_state, mousePixel);
        if (_state.IsPanning)
            CameraService::UpdatePan(_state, _window, mousePixel);
        if (_mmb->Type() == ActionType::End)
            CameraService::EndPan(_state);

        bool lmbDown         = (_lmb->Type() == ActionType::Start);
        bool lmbJustPressed  = lmbDown && !_lmbWasDown;
        bool lmbJustReleased = !lmbDown && _lmbWasDown;
        _lmbWasDown = lmbDown;

        bool ctrlHeld = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) ||
                        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RControl);

        bool copyDown  = ctrlHeld && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C);
        bool pasteDown = ctrlHeld && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::V);

        if (copyDown && !_copyWasDown)
            _controller.Copy(_state);

        if (pasteDown && !_pasteWasDown)
            _controller.Paste(_state, _window, mousePixel);

        _copyWasDown  = copyDown;
        _pasteWasDown = pasteDown;

        if (lmbJustPressed && !io.WantCaptureMouse) {
            if (!_state.SelectedObject.empty() && _state.HeldEntity == -1) {
                sf::Vector2f wp = CameraService::ScreenToWorld(_window, mousePixel, _state);
                int newEnt = GameObjectFactory::Create(world, _state.SelectedObject, wp.x, wp.y);
                if (newEnt != -1)
                    _controller.BeginDragNew(_state, newEnt, _window, mousePixel);
            } else {
                _controller.TryPickUp(_state, _window, mousePixel, ctrlHeld);
            }
        }

        if (lmbDown && _state.HeldEntity != -1)
            _controller.MoveHeld(_state, _window, mousePixel);

        if (lmbJustReleased)
            _controller.Drop(_state);
        
        bool rmbDown        = (_rmb->Type() == ActionType::Start);
        bool rmbJustPressed = rmbDown && !_rmbWasDown;
        _rmbWasDown = rmbDown;

        if (rmbJustPressed && !io.WantCaptureMouse)
            _controller.TryDelete(_window, mousePixel, _state);
    }
};