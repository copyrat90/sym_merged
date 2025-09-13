// SPDX-FileCopyrightText: Copyright 2021-2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: MIT

#include "sys/sram_rw.h"

#include <algorithm>
#include <limits>

namespace sym::sys
{

sram_rw::sram_rw(bn::span<const std::uint8_t> magic, unsigned location_0, unsigned location_1)
    : _location_0(location_0), _location_1(location_1)
{
    BN_ASSERT(magic.size() == MAGIC_LEN, "Invalid magic length: ", magic.size(), " (must be ", MAGIC_LEN, ")");

    BN_ASSERT(location_0 < SRAM_SIZE - sizeof(header), "Invalid location_0: ", location_0);
    BN_ASSERT(location_1 < SRAM_SIZE - sizeof(header), "Invalid location_1: ", location_1);

    bn::memcpy(_magic, magic.data(), sizeof(_magic));
}

sram_rw::sram_rw(bn::string_view magic, unsigned location_0, unsigned location_1)
    : _location_0(location_0), _location_1(location_1)
{
    // Allow ending with '\n' case with `MAGIC_LEN + 1` for convenience
    BN_ASSERT(magic.size() == MAGIC_LEN || magic.size() == MAGIC_LEN + 1, "Invalid magic length: ", magic.size(),
              " (must be ", MAGIC_LEN, ")");

    BN_ASSERT(location_0 < SRAM_SIZE - sizeof(header), "Invalid location_0: ", location_0);
    BN_ASSERT(location_1 < SRAM_SIZE - sizeof(header), "Invalid location_1: ", location_1);

    bn::memcpy(_magic, magic.data(), sizeof(_magic));
}

auto sram_rw::read_header_at(const int location) -> header
{
    header result;
    bn::sram::read_offset(result, location);
    return result;
}

bool sram_rw::validate_header(const header& header_) const
{
    return std::ranges::equal(bn::span<const std::uint8_t>(_magic), bn::span(header_.magic)) &&
           (ceil_to_multiple_of<sizeof(bit_stream_reader::word_type)>(header_.data_size) <= SRAM_SIZE);
}

void sram_rw::ensure_no_locations_overlap(int size) const
{
    const int save_locations_distance = bn::abs(_location_0 - _location_1);
    BN_ASSERT(save_locations_distance >= size, "Save location overlaps (", save_locations_distance, " < ", size, ")");
}

auto sram_rw::next_sequence() const -> std::uint8_t
{
    if (!_next_sequence.has_value())
        return 0;

    return _next_sequence.value();
}

void sram_rw::increase_next_sequence()
{
    if (!_next_sequence.has_value())
        _next_sequence = 1;
    else
        _next_sequence = _next_sequence.value() + 1;
}

void sram_rw::write_header(bn::span<std::uint8_t> span, bit_stream_writer::size_type logical_bytes_length)
{
    // Prepare the header (without crc32)
    header hdr;
    bn::memcpy(&hdr.magic, _magic, sizeof(hdr.magic));
    hdr.sequence = next_sequence();
    hdr.data_size = logical_bytes_length;

    // Copy the header
    bn::span<std::uint8_t> crc32_span(span.subspan(sizeof(std::uint32_t)));
    bn::memcpy(crc32_span.data(), reinterpret_cast<std::uint8_t*>(&hdr) + sizeof(std::uint32_t),
               sizeof(header) - sizeof(std::uint32_t));

    // Write crc32 checksum
    const std::uint32_t crc32 = crc32_fast(crc32_span.data(), crc32_span.size_bytes());
    bn::memcpy(span.data(), &crc32, sizeof(std::uint32_t));
}

bool sram_rw::sequence_greater_than(std::uint8_t a, std::uint8_t b)
{
    return ((a > b) && (a - b <= std::numeric_limits<std::uint8_t>::max() / 2)) ||
           ((a < b) && (b - a > std::numeric_limits<std::uint8_t>::max() / 2));
}

} // namespace sym::sys
