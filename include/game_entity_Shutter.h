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
    Shutter(bn::fixed_point position, bool isOpened, int textNumber);

    Shutter(Shutter&& other) = delete;
    Shutter& operator=(Shutter&& other) = delete;

    Shutter(const Shutter& other) = delete;
    Shutter& operator=(const Shutter& other) = delete;

    void AllocateGraphicResource(int z_order) final;
    void FreeGraphicResource() final;

    void Update() final;

    void InitShutterOpenAction();
    void InitShutterCloseAction();

private:
    bn::optional<bn::sprite_animate_action<4>> action_;
};

} // namespace sym::game::entity
