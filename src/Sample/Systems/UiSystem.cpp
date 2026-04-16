#include "UiSystem.h"

UiSystem::UiSystem(World &world)
    : ISystem(world),
      _positions(world.GetStorage<PositionComponent>()),
      _configs(world.GetStorage<SpawnerConfigComponent>()),
      _allWithPosition(FilterBuilder(world).With<PositionComponent>().Build()),
      _configFilter(FilterBuilder(world).With<SpawnerConfigComponent>().Build()) {
}

void UiSystem::OnUpdate() {
    for (int cfgEnt: _configFilter) {
        auto &cfg = _configs.Get(cfgEnt);

        ImGui::Begin("Game Settings");
        ImGui::Text("Asteroid Spawning");
        ImGui::SliderInt("Min Spawn Delay (ms)", &cfg.MinSpawnTimeMs, 100, 5000);
        ImGui::SliderInt("Max Spawn Delay (ms)", &cfg.MaxSpawnTimeMs, 100, 5000);

        if (ImGui::Button("Spawn Asteroid Now")) {
            cfg.ForceSpawn = true;
        }
        ImGui::End();
    }

    ImGui::Begin("Entity Inspector");
    if (ImGui::BeginTable("Entities", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
        ImGui::TableSetupColumn("ID");
        ImGui::TableSetupColumn("Pos X");
        ImGui::TableSetupColumn("Pos Y");
        ImGui::TableHeadersRow();

        for (int ent: _allWithPosition) {
            auto &pos = _positions.Get(ent);
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%d", ent);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%.1f", pos.X);
            ImGui::TableSetColumnIndex(2);
            ImGui::Text("%.1f", pos.Y);
        }
        ImGui::EndTable();
    }
    ImGui::End();
}
