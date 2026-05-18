#pragma once
#include <functional>
#include <unordered_map>
#include <typeindex>
#include <nlohmann/json.hpp>

#include "IComponentSerializer.h"
#include "SerializedEntity.h"
#include "ComponentSerializerFuncs.h"
#include "../../Ecs/World/World.h"

using json = nlohmann::json;

class ComponentSerializer final : public IComponentSerializer {
    World& _world;

    std::unordered_map<std::type_index, std::function<bool(int, SerializedComponent&)>>       _serializer;
    std::unordered_map<std::type_index, std::function<bool(int, const SerializedComponent&)>> _deserializer;
    std::unordered_map<size_t, std::type_index> _hashToTypeIdxMap;

public:
    explicit ComponentSerializer(World& world) : _world(world) {}

    template <typename T>
    void Register() {
        const std::type_index idx = typeid(T);

        _serializer[idx] = [this](int entity, SerializedComponent& out) -> bool {
            auto& storage = _world.GetStorage<T>();
            if (storage.Has(entity)) {
                SerializeComponent(storage.Get(entity), out);
                return true;
            }
            return false;
        };

        _deserializer[idx] = [this](int entity, const SerializedComponent& in) -> bool {
            auto& storage = _world.GetStorage<T>();
            if (!storage.Has(entity))
                DeserializeComponent(in, storage.Add(entity, T{}));
            else
                DeserializeComponent(in, storage.Get(entity));
            return true;
        };

        _hashToTypeIdxMap.emplace(idx.hash_code(), idx);
    }

    bool Serialize(int entity, std::type_index type, SerializedComponent& out) const override {
        auto it = _serializer.find(type);
        if (it == _serializer.end()) return false;
        return it->second(entity, out);
    }

    bool Deserialize(int entity, const SerializedComponent& data) override {
        auto it = _deserializer.find(data.Type);
        if (it == _deserializer.end()) return false;
        return it->second(entity, data);
    }

    const std::unordered_map<size_t, std::type_index>& HashToTypeIdxMap() const override {
        return _hashToTypeIdxMap;
    }

    static json SerializeAny(const std::any& value) {
        if (value.type() == typeid(int))         return {{"type", typeid(int).hash_code()},         {"value", std::any_cast<int>(value)}};
        if (value.type() == typeid(float))       return {{"type", typeid(float).hash_code()},       {"value", std::any_cast<float>(value)}};
        if (value.type() == typeid(bool))        return {{"type", typeid(bool).hash_code()},        {"value", std::any_cast<bool>(value)}};
        if (value.type() == typeid(std::string)) return {{"type", typeid(std::string).hash_code()}, {"value", std::any_cast<std::string>(value)}};
        return {{"type", 0}, {"value", nullptr}};
    }

    static json SerializeField(const SerializedField& field) {
        return {{"name", field.Name}, {"data", SerializeAny(field.Value)}};
    }

    static json SerializeSerializedComponent(const SerializedComponent& comp) {
        json fields = json::array();
        for (const auto& f : comp.Fields) fields.push_back(SerializeField(f));
        return {{"type", comp.Type.hash_code()}, {"fields", fields}};
    }

    static json SerializeEntity(const SerializedEntity& entity) {
        json j;
        j["components"] = json::array();
        for (const auto& [type, comp] : entity.Components)
            j["components"].push_back(SerializeSerializedComponent(comp));
        return j;
    }

    // Используем EntityCount() — нужно добавить в World.h (см. ниже)
    json SerializeWorld() const {
        json j;
        for (int id = 0; id < _world.EntityCount(); id++) {
            if (!_world.IsEntityAlive(id)) continue;
            SerializedEntity serializedEntity;
            for (const auto& [hash, type] : _hashToTypeIdxMap) {
                SerializedComponent comp;
                if (Serialize(id, type, comp))
                    serializedEntity.Components[type] = std::move(comp);
            }
            if (!serializedEntity.Components.empty())
                j["entities"][std::to_string(id)] = SerializeEntity(serializedEntity);
        }
        return j;
    }
};