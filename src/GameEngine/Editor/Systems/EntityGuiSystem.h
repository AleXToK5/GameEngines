#pragma once
#include <imgui.h>
#include "../../../Ecs/Systems/ISystem.h"
#include "../../../Ecs/Filter/FilterBuilder.h"
#include "../../../Sample/Physics/TransformComponent.h"
#include "../../../Sample/Graphics/SpriteComponent.h"

class EditorEntityGuiSystem final : public ISystem {
    ComponentStorage<TransformComponent>& _transforms;
    ComponentStorage<SpriteComponent>& _sprites;
    Filter _filter;

public:
    EditorEntityGuiSystem(World& world)
        : ISystem(world),
          _transforms(world.GetStorage<TransformComponent>()),
          _sprites(world.GetStorage<SpriteComponent>()),
          _filter(FilterBuilder(world).With<TransformComponent>().Build()) {}

    void OnInit() override {}

    void OnUpdate() override {
        ImGui::SetNextWindowPos({ImGui::GetIO().DisplaySize.x - 270.f, 10.f}, ImGuiCond_Once);
        ImGui::SetNextWindowSize({260.f, 400.f}, ImGuiCond_Once);
        ImGui::Begin("Entity Inspector");

        ImGui::Text("Entities: %d", _transforms.Count());
        ImGui::Separator();

        if (ImGui::BeginTable("EntTable", 4,
            ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY,
            {0.f, 300.f})) {

            ImGui::TableSetupColumn("ID",   ImGuiTableColumnFlags_WidthFixed, 30.f);
            ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("X",    ImGuiTableColumnFlags_WidthFixed, 55.f);
            ImGui::TableSetupColumn("Y",    ImGuiTableColumnFlags_WidthFixed, 55.f);
            ImGui::TableHeadersRow();

            for (int e : _filter) {
                const auto& t = _transforms.Get(e);
                std::string typeName = _sprites.Has(e) ? _sprites.Get(e).TextureName : "?";

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0); ImGui::Text("%d", e);
                ImGui::TableSetColumnIndex(1); ImGui::TextUnformatted(typeName.c_str());
                ImGui::TableSetColumnIndex(2); ImGui::Text("%.0f", t.X);
                ImGui::TableSetColumnIndex(3); ImGui::Text("%.0f", t.Y);
            }
            ImGui::EndTable();
        }
        ImGui::End();
    }
};