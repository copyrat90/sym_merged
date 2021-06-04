#pragma once

#include "game_entity_IOpenableEntity.h"

#include <bn_fixed_rect.h>
#include <bn_sprite_animate_actions.h>

namespace sym::game::entity
{

class Door final : public IOpenableEntity
{
public:
    /**
     * @brief Constructor.
     *
     * @param textNumber number which is shown above the door.
     */
    Door(bn::fixed_point position, bool isOpened, int textNumber);

    Door(Door&& other) = delete;
    Door& operator=(Door&& other) = delete;

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
