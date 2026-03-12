#ifndef DEMO_GUI_H
#define DEMO_GUI_H

#include <vector>
#include <memory>
#include <string>
#include "imgui.h"
#include "Logo.h"
#include "Text.h"

class Gui {
    // Индекс текущего выбранного лого
    int  _selectedTexture = 0;

    // Буфер для редактирования текста (вместо "Demo text")
    char _textBuffer[255] = "Токарев Александр ИУ3-22М";

    // Рабочие переменные для ImGui (синхронизируются с Logo)
    float _velocity[2]  = {200.f, 150.f};
    float _scale        = 0.5f;
    float _color[3]     = {1.f, 1.f, 1.f};

    void SyncFromLogo(const std::shared_ptr<Logo>& logo);

public:
    void Draw(std::shared_ptr<Logo> logo, std::shared_ptr<Text> text);
};

#endif //DEMO_GUI_H