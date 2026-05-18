#pragma once
#include <tuple>

// Описывает одно поле структуры: имя и указатель на член
template <typename Struct, typename T>
struct FieldInfo {
    const char* name;
    T Struct::* member;
};

// Макрос для удобной регистрации поля
#define FIELD(name) FieldInfo<TypeAlias, decltype(TypeAlias::name)>{ #name, &TypeAlias::name }

// Базовый шаблон — не определён намеренно, чтобы поймать незарегистрированные типы
template <typename T>
struct StructTraits;

// Макрос регистрации компонента с полями
// Пример: COMPONENT(TransformComponent, FIELD(X), FIELD(Y))
#define COMPONENT(Type, ...)                          \
template<>                                        \
struct StructTraits<Type> {                       \
using TypeAlias = Type;                       \
static constexpr auto fields = std::make_tuple(__VA_ARGS__); \
};