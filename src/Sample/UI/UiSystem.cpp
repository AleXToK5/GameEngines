#include "UiSystem.h"

UiSystem::UiSystem(World &world)
    : ISystem(world),
      _transforms(world.GetStorage<TransformComponent>()),
      _settings(world.GetStorage<RenderSettingsComponent>()),
      _allWithTransform(FilterBuilder(world).With<TransformComponent>().Build()),
      _settingsFilter(FilterBuilder(world).With<RenderSettingsComponent>().Build()) {
}

void UiSystem::OnInit() {
    int e = world.CreateEntity();
    _settings.Add(e, {true, false, true});
}

void UiSystem::OnUpdate() {
    RenderSettingsComponent *currentSettings = nullptr;
    for (int ent: _settingsFilter) {
        currentSettings = &_settings.Get(ent);
        break;
    }

    if (currentSettings) {
        ImGui::Begin("Render Modes");
        ImGui::Checkbox("Draw Textures", &currentSettings->DrawTextures);
        ImGui::Checkbox("Draw Colliders", &currentSettings->DrawColliders);
        ImGui::Checkbox("Draw Grid 64x64", &currentSettings->DrawGrid);
        ImGui::End();
    }

    ImGui::Begin("Entity Inspector");
    if (ImGui::BeginTable("Entities", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
        ImGui::TableSetupColumn("ID");
        ImGui::TableSetupColumn("Pos X");
        ImGui::TableSetupColumn("Pos Y");
        ImGui::TableHeadersRow();

        for (int ent: _allWithTransform) {
            auto &transform = _transforms.Get(ent);
            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%d", ent);

            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%.1f", transform.X);

            ImGui::TableSetColumnIndex(2);
            ImGui::Text("%.1f", transform.Y);
        }
        ImGui::EndTable();
    }
    ImGui::End();
}
