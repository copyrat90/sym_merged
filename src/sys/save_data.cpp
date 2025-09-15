// SPDX-FileCopyrightText: Copyright 2021-2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: MIT

#include "sys/save_data.h"

#include "ibn_sram_rw.h"

#include <type_traits>

namespace sym::sys
{

namespace
{

constexpr bn::string_view SAVE_MAGIC = "SYMM2";
constexpr unsigned SAVE_LOCATION_0 = 0;
constexpr unsigned SAVE_LOCATION_1 = 10240;

constexpr auto LANG_MIN = static_cast<lang>(0);
constexpr auto LANG_MAX = static_cast<lang>(31);
static_assert(lang::LANG_COUNT <= LANG_MAX);

constexpr std::uint8_t WORLD_IDX_MIN = 1;
constexpr std::uint8_t WORLD_IDX_MAX = 31;
constexpr std::uint8_t STAGE_IDX_MIN = 0;
constexpr std::uint8_t STAGE_IDX_MAX = 31;

constexpr std::uint32_t FOOTER = 0xDEADCAFE;

} // namespace

save_data::save_data()
{
    reset();
}

void save_data::reset()
{
    _lang = lang::ENG;
    _demo_cleared = false;
    reset_stage();
}

void save_data::reset_stage()
{
    _world_index = WORLD_IDX_MIN;
    _stage_index = STAGE_IDX_MIN;
}

void save_data::load()
{
    ibn::sram_rw rw(SAVE_MAGIC, SAVE_LOCATION_0, SAVE_LOCATION_1);

    // If read fails, it might be halfway-loaded (inconsistent state),
    // so we reset again.
    if (!rw.read(*this))
        reset();
}

void save_data::save()
{
    ibn::sram_rw rw(SAVE_MAGIC, SAVE_LOCATION_0, SAVE_LOCATION_1);
    rw.write(*this);
}

auto save_data::language() const -> lang
{
    return _lang;
}

void save_data::set_language(lang lang_)
{
    _lang = lang_;
}

bool save_data::demo_cleared() const
{
    return _demo_cleared;
}

void save_data::set_demo_cleared(bool demo_cleared)
{
    _demo_cleared = demo_cleared;
}

auto save_data::world_index() const -> std::uint8_t
{
    return _world_index;
}

void save_data::set_world_index(std::uint8_t world_index)
{
    _world_index = world_index;
}

auto save_data::stage_index() const -> std::uint8_t
{
    return _stage_index;
}

void save_data::set_stage_index(std::uint8_t stage_index)
{
    _stage_index = stage_index;
}

void save_data::measure(ibn::bit_stream_measurer& measurer) const
{
    measurer
        .write(_lang, LANG_MIN, LANG_MAX)                  // [0..31]: 5 bits
        .write(_demo_cleared)                              // bool: 1 bit
        .write(_world_index, WORLD_IDX_MIN, WORLD_IDX_MAX) // [1..31]: 5 bits
        .write(_stage_index, STAGE_IDX_MIN, STAGE_IDX_MAX) // [0..31]: 5 bits
        .write(FOOTER);                                    // 0xDEADCAFE: 32 bits
}

void save_data::write(ibn::bit_stream_writer& writer) const
{
    writer
        .write(_lang, LANG_MIN, LANG_MAX)                  // [0..31]: 5 bits
        .write(_demo_cleared)                              // bool: 1 bit
        .write(_world_index, WORLD_IDX_MIN, WORLD_IDX_MAX) // [1..31]: 5 bits
        .write(_stage_index, STAGE_IDX_MIN, STAGE_IDX_MAX) // [0..31]: 5 bits
        .write(FOOTER);                                    // 0xDEADCAFE: 32 bits
}

void save_data::read(ibn::bit_stream_reader& reader)
{
    std::uint32_t footer = 0;

    reader
        .read(_lang, LANG_MIN, LANG_MAX)                  // [0..31]: 5 bits
        .read(_demo_cleared)                              // bool: 1 bit
        .read(_world_index, WORLD_IDX_MIN, WORLD_IDX_MAX) // [1..31]: 5 bits
        .read(_stage_index, STAGE_IDX_MIN, STAGE_IDX_MAX) // [0..31]: 5 bits
        .read(footer);                                    // 0xDEADCAFE: 32 bits

    if (footer != FOOTER)
        reader.set_fail();
}

} // namespace sym::sys
