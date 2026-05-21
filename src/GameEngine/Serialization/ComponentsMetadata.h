#pragma once
#include <tuple>

template <typename Struct, typename T>
struct FieldInfo {
    const char* name;
    T Struct::* member;
};

#define FIELD(name) FieldInfo<TypeAlias, decltype(TypeAlias::name)>{ #name, &TypeAlias::name }

template <typename T>
struct StructTraits;

#define COMPONENT(Type, ...)                          \
template<>                                        \
struct StructTraits<Type> {                       \
using TypeAlias = Type;                       \
static constexpr auto fields = std::make_tuple(__VA_ARGS__); \
};