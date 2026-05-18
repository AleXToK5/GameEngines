#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include <SFML/System/Vector2.hpp>

struct RenderState {
    // Выбранный для размещения объект из палитры
    std::string SelectedObject;

    // Удерживаемые entity (drag одного или группы)
    // HeldEntity — первый схваченный, HeldOffset — смещение курсора от его позиции
    int HeldEntity = -1;
    sf::Vector2f HeldOffset;

    // Выделенные entity (Ctrl+клик или рамка)
    std::unordered_set<int> Selection;

    // Смещения каждого выделенного entity от точки захвата
    // key = entityId, value = offset от worldPos мыши в момент захвата
    std::unordered_map<int, sf::Vector2f> SelectionOffsets;

    // Буфер обмена: список {objName, relX, relY} относительно общего центра
    struct ClipboardEntry {
        std::string TextureName;
        std::string ObjectName;  // для GameObjectFactory
        sf::Vector2f RelPos;     // смещение от общего центра группы
    };
    std::vector<ClipboardEntry> Clipboard;

    // Зум и камера
    float Zoom = 1.f;
    sf::Vector2f CameraCenter;

    // Pan
    bool IsPanning = false;
    sf::Vector2i PanStartMouse;
    sf::Vector2f PanStartCamera;

    // Сохранение
    bool SaveRequested = false;
};