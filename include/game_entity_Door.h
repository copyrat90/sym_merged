#pragma once

#include "game_entity_IOpenableEntity.h"

#include <bn_fixed_rect.h>
#include <bn_sprite_animate_actions.h>

namespace sym::game::entity
{

class Door final : public IOpenableEntity
{
public:
    Door(bn::fixed_point position, bool isOpened);

    Door(Door&& other);
    Door& operator=(Door&& other);

    Door(const Door& other) = delete;
    Door& operator=(const Door& other) = delete;

    void FreeGraphicResource() final;

    void Update() final;

    void InitDoorOpenAction();
    void InitDoorCloseAction();

private:
    bn::optional<bn::sprite_animate_action<4>> action_;
};

} // namespace sym::game::entity
