#pragma once
#include <tuple>
#include "ComponentsMetadata.h"
#include "SerializedEntity.h"

template <typename T, typename Func>
void ForEachField(Func&& func) {
    std::apply([&](auto&&... field) {
        (func(field), ...);
    }, StructTraits<T>::fields);
}

template <typename T>
void SerializeComponent(const T& component, SerializedComponent& out) {
    out.Type = typeid(T);
    out.Fields.clear();
    ForEachField<T>([&](const auto& field) {
        SerializedField f;
        f.Name = field.name;
        f.Value = component.*(field.member);
        out.Fields.push_back(std::move(f));
    });
}

template <typename T>
void DeserializeComponent(const SerializedComponent& in, T& component) {
    for (const auto& field : in.Fields) {
        ForEachField<T>([&](const auto& f) {
            if (f.name == field.Name) {
                using FieldType = std::decay_t<decltype(component.*(f.member))>;
                if (field.Value.type() == typeid(FieldType))
                    component.*(f.member) = std::any_cast<FieldType>(field.Value);
            }
        });
    }
}