// SPDX-FileCopyrightText: Copyright 2021-2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: MIT

#include "sys/music.h"

#include "sys/configs.h"

#include "bn_music_items_info.h"
#include "ldtk_gen_project.h"

#include <bn_array.h>
#include <bn_fixed.h>
#include <bn_music.h>
#include <bn_music_item.h>

#include <algorithm>

namespace sym::sys::music
{

namespace
{

struct music_item
{
    const bn::music_item* item = nullptr;
    bn::fixed volume = 0;
};

constexpr int MUSICS_COUNT = bn::music_items_info::span.size();

constexpr bn::array<music_item, MUSICS_COUNT> MUSICS_LUT = [] constexpr -> bn::array<music_item, MUSICS_COUNT> {
    const auto& audio_configs_entity = get_config_entity(ldtk::gen::entity_ident::audio_configs);

    const auto music_names =
        audio_configs_entity.get_field(ldtk::gen::entity_field_ident::ENTITY_audio_configs_FIELD_music_names)
            .get<bn::span<const bn::string_view>>();
    const auto music_volumes =
        audio_configs_entity.get_field(ldtk::gen::entity_field_ident::ENTITY_audio_configs_FIELD_music_volumes)
            .get<bn::span<const bn::fixed>>();

    BN_ASSERT(MUSICS_COUNT == music_names.size(), "Name not specified for some musics");
    BN_ASSERT(MUSICS_COUNT == music_volumes.size(), "Volume not specified for some musics");

    bn::array<music_item, MUSICS_COUNT> result;
    for (int i = 0; i < MUSICS_COUNT; ++i)
    {
        const auto name = music_names[i];
        const auto iter = std::ranges::find_if(
            bn::music_items_info::span,
            [name](const bn::pair<bn::music_item, bn::string_view>& p) { return p.second == name; });
        BN_ASSERT(iter != bn::music_items_info::span.cend(), "Music not found");
        const auto& pair = *iter;

        result[i].item = &pair.first;
        result[i].volume = music_volumes[i];
    }

    return result;
}();

} // namespace

void play(ldtk::gen::music_kind kind)
{
    BN_ASSERT((int)kind < MUSICS_COUNT, "Invalid music_kind: ", (int)kind, " (max ", MUSICS_COUNT - 1, ")");

    const auto& item = MUSICS_LUT[(int)kind];
    bn::music::play(*item.item, item.volume);
}

} // namespace sym::sys::music
