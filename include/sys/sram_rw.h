// SPDX-FileCopyrightText: Copyright 2021-2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include "sys/bit_stream.h"
#include "sys/ceil_to_multiple_of.h"
#include "sys/crc32.h"

#include <alloca.h>

#include <bn_assert.h>
#include <bn_cstring.h>
#include <bn_math.h>
#include <bn_optional.h>
#include <bn_span.h>
#include <bn_sram.h>
#include <bn_string_view.h>

#include <concepts>
#include <cstdint>

namespace sym::sys
{

template <typename T>
concept sram_save_data =
    requires(T save_data, bit_stream_measurer& measurer, bit_stream_writer& writer, bit_stream_reader& reader) {
        { save_data.measure(measurer) } -> std::same_as<void>;
        { save_data.write(writer) } -> std::same_as<void>;
        { save_data.read(reader) } -> std::same_as<void>;
    };

class sram_rw final
{
private:
    static constexpr unsigned SRAM_SIZE = 32 * 1024;

    static constexpr unsigned MAGIC_LEN = 5;
    static constexpr unsigned DEFAULT_ALLOCA_SIZE = 256;

    struct header final
    {
        // checksum includes not only data, but also headers below
        std::uint32_t crc32;
        std::uint8_t magic[MAGIC_LEN];
        std::uint8_t sequence;
        std::uint16_t data_size;
    };

    static_assert(sizeof(header) % sizeof(bit_stream_writer::word_type) == 0,
                  "Header makes data portion not aligned to bit stream words");
    static_assert(__BIGGEST_ALIGNMENT__ >= sizeof(bit_stream_writer::word_type),
                  "`alloca()` is not aligned to bit stream words");

public:
    /// @brief Constructor.
    /// @param magic Magic string to uniquely distinguish your game (i.e. Game Code). Must be 5 bytes.
    /// @param location_0 First SRAM location to store the save data.
    /// @param location_1 Second SRAM location to store the save data.
    sram_rw(bn::span<const std::uint8_t> magic, unsigned location_0, unsigned location_1);

    /// @brief Constructor.
    /// @param magic Magic string to uniquely distinguish your game (i.e. Game Code). Must be 5 bytes.
    /// @param location_0 First SRAM location to store the save data.
    /// @param location_1 Second SRAM location to store the save data.
    sram_rw(bn::string_view magic, unsigned location_0, unsigned location_1);

public:
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstack-usage="

    /// @brief Writes the save data to the SRAM.
    /// @tparam SaveData Save data class that satisfies `sram_save_data` concept.
    /// @param save_data Save data to be saved.
    /// @param max_stack_buffer_size Maximum temporary stack buffer size to avoid allocating on the heap.
    template <sram_save_data SaveData>
    void write(const SaveData& save_data, unsigned max_stack_buffer_size = DEFAULT_ALLOCA_SIZE)
    {
        // Measure how much space required
        bit_stream_measurer measurer;
        save_data.measure(measurer);

        const unsigned raw_data_size = measurer.used_bytes();
        const unsigned ceiled_data_size = ceil_to_multiple_of<sizeof(bit_stream_writer::word_type)>(raw_data_size);
        const unsigned buffer_size = sizeof(header) + ceiled_data_size;

        BN_ASSERT(buffer_size <= SRAM_SIZE / 2, "Save data size too big: ", raw_data_size);
        ensure_no_locations_overlap(buffer_size);

        // `alloca()` on small sizes
        const bool use_stack_buffer = buffer_size <= max_stack_buffer_size;

        // Allocate temporary buffer (both would be aligned to 4 bytes)
        std::uint8_t* buffer;
        if (use_stack_buffer)
            buffer = static_cast<std::uint8_t*>(alloca(buffer_size));
        else
            buffer = new std::uint8_t[buffer_size];

        // Serialize from save data to the buffer
        bn::span<bit_stream_writer::word_type> data_span(
            reinterpret_cast<bit_stream_writer::word_type*>(buffer + sizeof(header)),
            ceiled_data_size / sizeof(bit_stream_writer::word_type));
        bit_stream_writer writer(data_span, raw_data_size);
        save_data.write(writer);
        writer.flush_final();

        // User must have correctly serialized their save data to `writer`
        BN_ASSERT(!writer.fail(), "Error serializing save data");

        // Write the header
        bn::span<std::uint8_t> buffer_span(buffer, buffer_size);
        write_header(buffer_span, raw_data_size);

        // Store to SRAM
        if (next_sequence() % 2 == 0)
            bn::sram::write_span_offset(buffer_span, _location_0);
        else
            bn::sram::write_span_offset(buffer_span, _location_1);

        // Deallocate temporary buffer
        if (!use_stack_buffer)
            delete[] buffer;

        increase_next_sequence();
    }

#pragma GCC diagnostic pop

    /// @brief Reads the save data from the SRAM.
    /// @tparam SaveData Save data class that satisfies `sram_save_data` concept.
    /// heap.
    /// @param save_data Save data to be loaded.
    /// @param max_stack_buffer_size Maximum temporary stack buffer size to avoid allocating on the heap.
    /// @return Whether the save data has been loaded or not.
    template <sram_save_data SaveData>
    bool read(SaveData& save_data, unsigned max_stack_buffer_size = DEFAULT_ALLOCA_SIZE)
    {
        // Look at both locations for headers to find the recent save
        header header_0 = read_header_at(_location_0);
        header header_1 = read_header_at(_location_1);

        if (validate_header(header_0) && validate_header(header_1))
        {
            const bool recent_is_0 = sequence_greater_than(header_0.sequence, header_1.sequence);
            if (recent_is_0)
            {
                if (read_at(save_data, _location_0, header_0, max_stack_buffer_size))
                    return true;
                else
                    return read_at(save_data, _location_1, header_1, max_stack_buffer_size);
            }
            else
            {
                if (read_at(save_data, _location_1, header_1, max_stack_buffer_size))
                    return true;
                else
                    return read_at(save_data, _location_0, header_0, max_stack_buffer_size);
            }
        }
        else if (validate_header(header_0))
        {
            return read_at(save_data, _location_0, header_0, max_stack_buffer_size);
        }
        else if (validate_header(header_1))
        {
            return read_at(save_data, _location_1, header_1, max_stack_buffer_size);
        }

        return false;
    }

private:
    auto read_header_at(const int location) -> header;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstack-usage="

    template <sram_save_data SaveData>
    bool read_at(SaveData& save_data, const int location, const header& header_, unsigned max_stack_buffer_size)
    {
        const unsigned data_location = location + sizeof(header);
        const unsigned raw_data_size = header_.data_size;
        const unsigned ceiled_data_size = ceil_to_multiple_of<sizeof(bit_stream_reader::word_type)>(raw_data_size);

        if (data_location + ceiled_data_size > SRAM_SIZE)
            return false;

        // `alloca()` on small sizes
        const unsigned buffer_size = sizeof(header) + ceiled_data_size;
        const bool use_stack_buffer = buffer_size <= max_stack_buffer_size;

        // Allocate temporary buffer (both would be aligned to 4 bytes)
        std::uint8_t* buffer;
        if (use_stack_buffer)
            buffer = static_cast<std::uint8_t*>(alloca(buffer_size));
        else
            buffer = new std::uint8_t[buffer_size];

        // Read to the temporary buffer
        bn::span<std::uint8_t> read_data_span(buffer + sizeof(header), ceiled_data_size);
        bn::sram::read_span_offset(read_data_span, data_location);

        // Validate crc32 checksum
        bn::span<std::uint8_t> crc32_span(buffer + sizeof(std::uint32_t),
                                          sizeof(header) - sizeof(std::uint32_t) + ceiled_data_size);
        bn::memcpy(crc32_span.data(), reinterpret_cast<const std::uint8_t*>(&header_) + sizeof(std::uint32_t),
                   sizeof(header) - sizeof(std::uint32_t));
        const std::uint32_t crc32 = crc32_fast(crc32_span.data(), crc32_span.size_bytes());
        bool success = crc32 == header_.crc32;

        if (success)
        {
            // Deserialize to the `save_data`
            bn::span<const std::uint32_t> data_span(
                reinterpret_cast<const bit_stream_reader::word_type*>(buffer + sizeof(header)),
                ceiled_data_size / sizeof(bit_stream_reader::word_type));
            bit_stream_reader reader(data_span, raw_data_size);
            save_data.read(reader);
            success = !reader.fail() && reader.unused_bytes() == 0;
        }

        // Deallocate temporary buffer
        if (!use_stack_buffer)
            delete[] buffer;

        if (success)
            _next_sequence = header_.sequence + 1;

        return success;
    }

#pragma GCC diagnostic pop

private:
    // Not a full check
    // (can't check crc32 without looking at data)
    bool validate_header(const header&) const;

    void ensure_no_locations_overlap(int size) const;

    auto next_sequence() const -> std::uint8_t;
    void increase_next_sequence();

    void write_header(bn::span<std::uint8_t> span, bit_stream_writer::size_type logical_bytes_length);

    static bool sequence_greater_than(std::uint8_t a, std::uint8_t b);

private:
    const int _location_0;
    const int _location_1;

    std::uint8_t _magic[MAGIC_LEN];

    bn::optional<std::uint8_t> _next_sequence;
};

} // namespace sym::sys
