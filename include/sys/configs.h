// SPDX-FileCopyrightText: Copyright 2021-2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include "ldtk_gen_enums.h"
#include "ldtk_gen_project.h"

namespace sym::sys
{

constexpr auto get_config_entity(ldtk::gen::entity_ident config_entity_ident) -> const ldtk::entity&
{
    static constexpr auto CONFIG_ENTITIES = ldtk::gen::gen_project.get_level(ldtk::gen::level_ident::configs)
                                                .get_layer(ldtk::gen::layer_ident::entities)
                                                .entity_instances();

    const auto configs_entity_iter =
        std::ranges::find_if(CONFIG_ENTITIES, [config_entity_ident](const ldtk::entity& entity) {
            return entity.identifier() == config_entity_ident;
        });

    BN_ASSERT(configs_entity_iter != CONFIG_ENTITIES.cend(), "config entity not found");
    return *configs_entity_iter;
}

} // namespace sym::sys
