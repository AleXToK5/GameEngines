#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include "RenderState.h"
#include "Constants.h"

class CameraService {
public:
    static sf::View BuildView(const sf::RenderWindow& window, const RenderState& state) {
        sf::View view;
        view.setSize(static_cast<sf::Vector2f>(window.getSize()));
        view.zoom(state.Zoom);
        view.setCenter(state.CameraCenter);
        return view;
    }

    static sf::Vector2f ScreenToWorld(const sf::RenderWindow& window,
                                      const sf::Vector2i& pixel,
                                      const RenderState& state) {
        return window.mapPixelToCoords(pixel, BuildView(window, state));
    }

    static void ApplyZoom(RenderState& state, float delta) {
        float newZoom = state.Zoom - delta * EditorConstants::ZoomStep;
        state.Zoom = std::clamp(newZoom, EditorConstants::ZoomMin, EditorConstants::ZoomMax);
    }

    static void BeginPan(RenderState& state, const sf::Vector2i& mousePos) {
        state.IsPanning      = true;
        state.PanStartMouse  = mousePos;
        state.PanStartCamera = state.CameraCenter;
    }

    static void UpdatePan(RenderState& state,
                          const sf::RenderWindow& window,
                          const sf::Vector2i& mousePos) {
        if (!state.IsPanning) return;

        sf::Vector2f delta = static_cast<sf::Vector2f>(state.PanStartMouse - mousePos);
        delta *= state.Zoom;
        state.CameraCenter = state.PanStartCamera + delta;
    }
    
    static void EndPan(RenderState& state) {
        state.IsPanning = false;
    }
};