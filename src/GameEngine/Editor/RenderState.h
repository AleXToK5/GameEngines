#pragma once
#include <string>
#include <SFML/System/Vector2.hpp>

struct RenderState {
    // Выбранный для размещения объект ("" = ничего не выбрано)
    std::string SelectedObject;

    // Объект "в руке" — подобранный для перемещения entity (-1 = нет)
    int HeldEntity = -1;

    // Смещение между центром entity и точкой подбора (для точного drag)
    sf::Vector2f HeldOffset;

    // Зум камеры
    float Zoom = 1.f;

    // Центр камеры в мировых координатах
    sf::Vector2f CameraCenter;

    // Нужно ли сохранить уровень в этот кадр
    bool SaveRequested = false;

    // Drag камеры: зажата ли средняя/правая кнопка для pan
    bool IsPanning = false;
    sf::Vector2i PanStartMouse;
    sf::Vector2f PanStartCamera;
};