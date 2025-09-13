// SPDX-FileCopyrightText: Copyright 2021-2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include <cstdint>
#include <type_traits>

namespace sym::sys
{

template <typename T>
struct make_unsigned_allow_bool
{
    using type = std::make_unsigned_t<T>;
};

template <>
struct make_unsigned_allow_bool<bool>
{
    using type = std::uint8_t;
};

template <typename T>
using make_unsigned_allow_bool_t = typename make_unsigned_allow_bool<T>::type;

} // namespace sym::sys
