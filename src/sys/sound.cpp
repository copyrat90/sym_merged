// SPDX-FileCopyrightText: Copyright 2021-2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: MIT

#include "sys/sound.h"

#include "sys/configs.h"

#include "bn_sound_items_info.h"

#include <bn_array.h>
#include <bn_fixed.h>
#include <bn_sound.h>
#include <bn_sound_item.h>

#include <algorithm>

namespace sym::sys::sound
{

namespace
{

struct sound_item
{
    const bn::sound_item* item = nullptr;
    bn::fixed volume = 0;
};

constexpr int SOUNDS_COUNT = bn::sound_items_info::span.size();

constexpr bn::array<sound_item, SOUNDS_COUNT> SOUNDS_LUT = [] constexpr -> bn::array<sound_item, SOUNDS_COUNT> {
    const auto& audio_configs_entity = get_config_entity(ldtk::gen::entity_ident::audio_configs);

    const auto sound_names =
        audio_configs_entity.get_field(ldtk::gen::entity_field_ident::ENTITY_audio_configs_FIELD_sound_names)
            .get<bn::span<const bn::string_view>>();
    const auto sound_volumes =
        audio_configs_entity.get_field(ldtk::gen::entity_field_ident::ENTITY_audio_configs_FIELD_sound_volumes)
            .get<bn::span<const bn::fixed>>();

    BN_ASSERT(SOUNDS_COUNT == sound_names.size(), "Name not specified for some sounds");
    BN_ASSERT(SOUNDS_COUNT == sound_volumes.size(), "Volume not specified for some sounds");

    bn::array<sound_item, SOUNDS_COUNT> result;
    for (int i = 0; i < SOUNDS_COUNT; ++i)
    {
        const auto name = sound_names[i];
        const auto iter = std::ranges::find_if(
            bn::sound_items_info::span,
            [name](const bn::pair<bn::sound_item, bn::string_view>& p) { return p.second == name; });
        BN_ASSERT(iter != bn::sound_items_info::span.cend(), "Sound not found");
        const auto& pair = *iter;

        result[i].item = &pair.first;
        result[i].volume = sound_volumes[i];
    }

    return result;
}();

} // namespace

auto play(ldtk::gen::sound_kind kind) -> bn::sound_handle
{
    BN_ASSERT((int)kind < SOUNDS_COUNT, "Invalid sound_kind: ", (int)kind, " (max ", SOUNDS_COUNT - 1, ")");

    const auto& item = SOUNDS_LUT[(int)kind];
    return bn::sound::play(*item.item, item.volume);
}

} // namespace sym::sys::sound
