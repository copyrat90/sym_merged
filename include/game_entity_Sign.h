#pragma once

#include "game_entity_IEntity.h"

namespace sym::game::entity
{

class Sign : public IEntity
{
public:
    Sign(bn::fixed_point position);

    Sign(Sign&& other) = delete;
    Sign& operator=(Sign&& other) = delete;

    Sign(const Sign& other) = delete;
    Sign& operator=(const Sign& other) = delete;
};

} // namespace sym::game::entity
