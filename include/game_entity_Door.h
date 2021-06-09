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
    Door(bn::fixed_point position, bool isOpenedByDefault, int textNumber);

    Door(Door&& other) = delete;
    Door& operator=(Door&& other) = delete;

    Door(const Door& other) = delete;
    Door& operator=(const Door& other) = delete;

    void AllocateGraphicResource(int z_order) final;
    void FreeGraphicResource() final;

    void Update() final;

    /**
     * @brief Toggle opened (also animates door)
     *
     * @return `true` if the door is opened
     */
    [[maybe_unused]] bool ToggleOpened() final;

private:
    bn::optional<bn::sprite_animate_action<4>> action_;

    void InitDoorOpenAction_();
    void InitDoorCloseAction_();
};

} // namespace sym::game::entity
