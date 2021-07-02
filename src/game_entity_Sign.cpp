#include "game_entity_Sign.h"

#include "bn_sprite_items_spr_sign.h"

namespace sym::game::entity
{

namespace
{

constexpr bn::fixed_rect RELATIVE_INTERACT_RANGE = {0, 0, 16, 12};

}

Sign::Sign(bn::fixed_point position) : IEntity(position, RELATIVE_INTERACT_RANGE, &bn::sprite_items::spr_sign)
{
}

} // namespace sym::game::entity
