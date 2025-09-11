// SPDX-FileCopyrightText: Copyright 2021-2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include <type_traits>

#define ENUM_AS_FLAGS(Enum) \
    static_assert(std::is_enum<Enum>::value, "Template argument for Enum is not an enum."); \
\
    constexpr bool operator!(Enum a) \
    { \
        using Int = typename std::underlying_type<Enum>::type; \
        return !static_cast<Int>(a); \
    } \
\
    constexpr Enum operator~(Enum a) \
    { \
        using Int = typename std::underlying_type<Enum>::type; \
        return static_cast<Enum>(~static_cast<Int>(a)); \
    } \
\
    constexpr Enum operator|(Enum a, Enum b) \
    { \
        using Int = typename std::underlying_type<Enum>::type; \
        return static_cast<Enum>(static_cast<Int>(a) | static_cast<Int>(b)); \
    } \
\
    constexpr Enum& operator|=(Enum& a, Enum b) \
    { \
        return a = a | b; \
    } \
\
    constexpr Enum operator&(Enum a, Enum b) \
    { \
        using Int = typename std::underlying_type<Enum>::type; \
        return static_cast<Enum>(static_cast<Int>(a) & static_cast<Int>(b)); \
    } \
\
    constexpr Enum& operator&=(Enum& a, Enum b) \
    { \
        return a = a & b; \
    } \
\
    constexpr Enum operator^(Enum a, Enum b) \
    { \
        using Int = typename std::underlying_type<Enum>::type; \
        return static_cast<Enum>(static_cast<Int>(a) ^ static_cast<Int>(b)); \
    } \
\
    constexpr Enum& operator^=(Enum& a, Enum b) \
    { \
        return a = a ^ b; \
    }
