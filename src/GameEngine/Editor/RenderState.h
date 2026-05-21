#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include <SFML/System/Vector2.hpp>

struct RenderState {
    std::string SelectedObject;

    int HeldEntity = -1;
    sf::Vector2f HeldOffset;

    std::unordered_set<int> Selection;

    std::unordered_map<int, sf::Vector2f> SelectionOffsets;

    struct ClipboardEntry {
        std::string TextureName;
        std::string ObjectName;
        sf::Vector2f RelPos;
    };
    std::vector<ClipboardEntry> Clipboard;

    float Zoom = 1.f;
    sf::Vector2f CameraCenter;

    bool IsPanning = false;
    sf::Vector2i PanStartMouse;
    sf::Vector2f PanStartCamera;
    
    bool SaveRequested = false;
};