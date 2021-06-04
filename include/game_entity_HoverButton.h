#pragma once

#include "game_entity_IButtonEntity.h"

#include <bn_optional.h>
#include <bn_sprite_animate_actions.h>

namespace sym::game::entity
{

class HoverButton final : public IButtonEntity
{
public:
    HoverButton(bn::fixed_point position, int textNumber, bool isOnByDefault);

    HoverButton(HoverButton&& other) = delete;
    HoverButton& operator=(HoverButton&& other) = delete;

    HoverButton(const HoverButton& other) = delete;
    HoverButton& operator=(const HoverButton& other) = delete;

    void AllocateGraphicResource(int z_order) final;
    void FreeGraphicResource() final;

    void Update() final;

    void InitButtonOnAction();
    void InitButtonOffAction();

private:
    bn::optional<bn::sprite_animate_action<2>> action_;
};

} // namespace sym::game::entity
