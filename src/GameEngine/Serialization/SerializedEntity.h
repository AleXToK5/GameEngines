#pragma once
#include <string>
#include <vector>
#include <map>
#include <any>
#include <typeindex>

struct SerializedField {
    std::string Name;
    std::any Value;
};

struct SerializedComponent {
    std::type_index Type = typeid(void);
    std::vector<SerializedField> Fields;
};

struct SerializedEntity {
    std::map<std::type_index, SerializedComponent> Components;
};