#pragma once

#include "game_entity_Entity.h"

#include <bn_optional.h>
#include <bn_sprite_ptr.h>

namespace sym::game::entity
{

class Symbol final : Entity
{
public:
    void FreeGraphicResources() final;
    void AllocateGraphicResources() final;

private:
    bn::optional<bn::sprite_ptr> sprite_;
};

} // namespace sym::game::entity
