#pragma once

#include "game_entity_IEntity.h"

namespace sym::game::entity
{

class BlackHole final : public IEntity
{
public:
    BlackHole(bn::fixed_point position);

    BlackHole(BlackHole&& other) = delete;
    BlackHole& operator=(BlackHole&& other) = delete;

    BlackHole(const BlackHole& other) = delete;
    BlackHole& operator=(const BlackHole& other) = delete;
};

} // namespace sym::game::entity
