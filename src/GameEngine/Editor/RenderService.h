#pragma once
#include <cmath>
#include <SFML/Graphics.hpp>
#include "RenderState.h"
#include "CameraService.h"
#include "Constants.h"
#include "../Assets/AssetManager.h"

class RenderService {
public:
    static void DrawGrid(sf::RenderWindow& window, const RenderState& state) {
        sf::View view = CameraService::BuildView(window, state);
        window.setView(view);

        float ts     = EditorConstants::TileSize;
        float left   = view.getCenter().x - view.getSize().x / 2.f;
        float right  = view.getCenter().x + view.getSize().x / 2.f;
        float top    = view.getCenter().y - view.getSize().y / 2.f;
        float bottom = view.getCenter().y + view.getSize().y / 2.f;

        sf::Color gridColor(255, 255, 255, 40);

        float startX = std::floor(left / ts) * ts;
        for (float x = startX; x <= right + ts; x += ts) {
            sf::Vertex line[] = {
                sf::Vertex({x, top},    gridColor),
                sf::Vertex({x, bottom}, gridColor)
            };
            window.draw(line, 2, sf::PrimitiveType::Lines);
        }

        float startY = std::floor(top / ts) * ts;
        for (float y = startY; y <= bottom + ts; y += ts) {
            sf::Vertex line[] = {
                sf::Vertex({left,  y}, gridColor),
                sf::Vertex({right, y}, gridColor)
            };
            window.draw(line, 2, sf::PrimitiveType::Lines);
        }
    }

    static void DrawPreview(sf::RenderWindow& window,
                            const RenderState& state,
                            const AssetManager& assets,
                            const sf::Vector2i& mousePixel) {
        if (state.SelectedObject.empty()) return;
        if (state.HeldEntity != -1) return;

        auto it = EditorConstants::ObjectTextures.find(state.SelectedObject);
        if (it == EditorConstants::ObjectTextures.end()) return;

        sf::View view = CameraService::BuildView(window, state);
        sf::Vector2f worldPos = window.mapPixelToCoords(mousePixel, view);

        float ts = EditorConstants::TileSize;
        float snappedX = std::floor(worldPos.x / ts) * ts + ts / 2.f;
        float snappedY = std::floor(worldPos.y / ts) * ts + ts / 2.f;

        try {
            const sf::Texture& tex = assets.GetTexture(it->second);
            sf::Sprite sprite(tex);
            sprite.setOrigin({tex.getSize().x / 2.f, tex.getSize().y / 2.f});
            sprite.setPosition({snappedX, snappedY});
            sprite.setColor(sf::Color(255, 255, 255, 120));
            window.setView(view);
            window.draw(sprite);
        } catch (...) {}
    }
};