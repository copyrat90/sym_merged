// SPDX-FileCopyrightText: Copyright 2021-2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include "sys/lang.h"

#include <cstdint>

namespace sym::sys
{

class bit_stream_measurer;
class bit_stream_writer;
class bit_stream_reader;

class save_data final
{
public:
    save_data();

public:
    void reset();
    void reset_stage();

    void load();
    void save();

public:
    auto language() const -> lang;
    void set_language(lang);

    bool demo_cleared() const;
    void set_demo_cleared(bool);

    auto world_index() const -> std::uint8_t;
    void set_world_index(std::uint8_t);

    auto stage_index() const -> std::uint8_t;
    void set_stage_index(std::uint8_t);

public: // `sram_rw` uses these to save/load
    void measure(bit_stream_measurer& measurer) const;
    void write(bit_stream_writer& writer) const;
    void read(bit_stream_reader& reader);

private:
    lang _lang;
    bool _demo_cleared;
    std::uint8_t _world_index;
    std::uint8_t _stage_index;
};

} // namespace sym::sys
