#pragma once
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>
#include "../../../Ecs/Systems/ISystem.h"
#include "../../../GameEngine/Assets/AssetManager.h"
#include "../RenderState.h"
#include "../Constants.h"

class EditorGuiSystem final : public ISystem {
    const AssetManager& _assets;
    RenderState& _state;

    static constexpr float BtnSize = 56.f;

public:
    EditorGuiSystem(World& world, const AssetManager& assets, RenderState& state)
        : ISystem(world), _assets(assets), _state(state) {}

    void OnInit() override {}

    void OnUpdate() override {
        ImGui::SetNextWindowPos({10.f, 10.f}, ImGuiCond_Once);
        ImGui::SetNextWindowSize({160.f, 0.f}, ImGuiCond_Once);
        ImGui::Begin("Assets", nullptr, ImGuiWindowFlags_NoResize);

        ImGui::Text("Objects:");
        ImGui::Separator();

        for (const auto& objName : EditorConstants::PlaceableObjects) {
            auto texIt = EditorConstants::ObjectTextures.find(objName);
            if (texIt == EditorConstants::ObjectTextures.end()) continue;

            const sf::Texture* tex = nullptr;
            try {
                tex = &_assets.GetTexture(texIt->second);
            } catch (...) {
                bool selected = (_state.SelectedObject == objName);
                if (selected) ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.7f, 0.3f, 1.f));
                if (ImGui::Button(objName.c_str(), {130.f, 30.f}))
                    _state.SelectedObject = objName;
                if (selected) ImGui::PopStyleColor();
                ImGui::Separator();
                continue;
            }

            sf::Sprite sprite(*tex);
            float scale = std::min(
                BtnSize / static_cast<float>(tex->getSize().x),
                BtnSize / static_cast<float>(tex->getSize().y)
            );
            sprite.setScale({scale, scale});
            const auto& size = sprite.getGlobalBounds().size;

            bool selected = (_state.SelectedObject == objName);
            if (selected) {
                ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.3f, 0.7f, 0.3f, 1.f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.8f, 0.4f, 1.f));
            }

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(
                (BtnSize - size.x) / 2.f + 3.f,
                (BtnSize - size.y) / 2.f + 3.f
            ));

            std::string btnId = "##obj_" + objName;
            if (ImGui::ImageButton(btnId.c_str(), sprite, {size.x, size.y})) {
                _state.SelectedObject = (_state.SelectedObject == objName) ? "" : objName;
            }

            ImGui::PopStyleVar();

            if (selected) ImGui::PopStyleColor(2);

            float textW = ImGui::CalcTextSize(objName.c_str()).x;
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (130.f - textW) / 2.f);
            ImGui::Text("%s", objName.c_str());
            ImGui::Separator();
        }

        ImGui::Spacing();
        if (ImGui::Button("Save Level", {130.f, 35.f})) {
            _state.SaveRequested = true;
        }

        if (!_state.SelectedObject.empty()) {
            ImGui::Spacing();
            ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), "Selected:");
            ImGui::TextWrapped("%s", _state.SelectedObject.c_str());
            ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.f), "Click scene to place");
        }

        ImGui::End();
    }
};