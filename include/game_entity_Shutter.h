#pragma once

#include "game_entity_IOpenableEntity.h"

#include <bn_fixed_rect.h>
#include <bn_sprite_animate_actions.h>

namespace sym::game::entity
{

class Shutter final : public IOpenableEntity
{
public:
    /**
     * @brief Constructor.
     *
     * @param textNumber number which is shown above the Shutter.
     */
    Shutter(bn::fixed_point position, bool isOpenedByDefault, int textNumber);

    Shutter(Shutter&& other) = delete;
    Shutter& operator=(Shutter&& other) = delete;

    Shutter(const Shutter& other) = default;
    Shutter& operator=(const Shutter& other) = default;

    void AllocateGraphicResource(int z_order) final;
    void FreeGraphicResource() final;

    void Update() final;

    /**
     * @brief Toggle opened (also animates shutter)
     *
     * @return `true` if the shutter is opened
     */
    [[maybe_unused]] bool ToggleOpened() final;

private:
    bn::optional<bn::sprite_animate_action<4>> action_;

    void InitShutterOpenAction_();
    void InitShutterCloseAction_();
};

} // namespace sym::game::entity
