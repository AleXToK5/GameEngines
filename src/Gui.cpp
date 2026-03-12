#include "Gui.h"
#include <cstring>
#include <cmath>

void Gui::SyncFromLogo(const std::shared_ptr<Logo>& logo)
{
    auto vel   = logo->GetVelocity();
    auto color = logo->GetColor();

    _velocity[0] = std::abs(vel.x);
    _velocity[1] = std::abs(vel.y);
    _scale       = logo->GetScale();
    _color[0]    = color.r / 255.f;
    _color[1]    = color.g / 255.f;
    _color[2]    = color.b / 255.f;
}

void Gui::Draw(std::shared_ptr<Logo> logo, std::shared_ptr<Text> text)
{
    static bool firstFrame = true;
    if (firstFrame) {
        SyncFromLogo(logo);
        firstFrame = false;
    }

    ImGui::Begin("Settings");

    // --- Выбор текстуры лого ---
    std::vector<std::string> names;
    for (int i = 0; i < logo->GetTextureCount(); i++)
        names.push_back("Logo " + std::to_string(i + 1));

    std::vector<const char*> namePtrs;
    for (auto& n : names) namePtrs.push_back(n.c_str());

    int prev = _selectedTexture;
    ImGui::Text("Current logo:");
    ImGui::Combo("##logo", &_selectedTexture, namePtrs.data(), (int)namePtrs.size());
    if (_selectedTexture != prev)
        logo->SetTextureIndex(_selectedTexture);  // меняем текстуру, лого продолжает лететь

    ImGui::Separator();

    // --- Скорость ---
    ImGui::Text("Velocity:");
    if (ImGui::SliderFloat2("##velocity", _velocity, 0.f, 600.f))
    {
        auto vel = logo->GetVelocity();
        float signX = (vel.x >= 0.f) ? 1.f : -1.f;
        float signY = (vel.y >= 0.f) ? 1.f : -1.f;
        logo->SetVelocity({signX * _velocity[0], signY * _velocity[1]});
    }

    ImGui::Separator();

    // --- Масштаб ---
    ImGui::Text("Scale:");
    if (ImGui::SliderFloat("##scale", &_scale, 0.1f, 3.0f))
        logo->SetScale(_scale);

    ImGui::Separator();

    // --- Цвет ---
    ImGui::Text("Color:");
    if (ImGui::ColorEdit3("##color", _color))
    {
        logo->SetColor(sf::Color(
            (uint8_t)(_color[0] * 255),
            (uint8_t)(_color[1] * 255),
            (uint8_t)(_color[2] * 255)
        ));
    }

    ImGui::Separator();

    // --- Сброс позиции ---
    if (ImGui::Button("Reset position"))
        logo->ResetPosition();

    ImGui::Separator();

    // --- Поле ввода текста ---
    ImGui::Text("Display text:");
    if (ImGui::InputText("##text", _textBuffer, sizeof(_textBuffer)))
        text->SetString(sf::String::fromUtf8(
            _textBuffer, _textBuffer + strlen(_textBuffer)));

    ImGui::End();
}