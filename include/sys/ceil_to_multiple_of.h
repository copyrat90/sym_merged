// SPDX-FileCopyrightText: Copyright 2021-2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include <bit>
#include <concepts>
#include <cstddef>

namespace sym::sys
{

template <std::size_t PowerOfTwo, std::integral Int>
    requires(std::has_single_bit(PowerOfTwo))
constexpr Int ceil_to_multiple_of(Int size)
{
    return static_cast<Int>((size + (PowerOfTwo - 1)) & ~(PowerOfTwo - 1));
}

} // namespace sym::sys
