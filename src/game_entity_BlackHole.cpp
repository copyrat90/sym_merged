#include "game_entity_BlackHole.h"

#include "bn_sprite_items_spr_black_hole.h"

namespace sym::game::entity
{

namespace
{

constexpr bn::fixed_rect RELATIVE_INTERACT_RANGE = {0, 0, 64, 36};

}

BlackHole::BlackHole(bn::fixed_point position)
    : IEntity(position, RELATIVE_INTERACT_RANGE, &bn::sprite_items::spr_black_hole)
{
}

} // namespace sym::game::entity
