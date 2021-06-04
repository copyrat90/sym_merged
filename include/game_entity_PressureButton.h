#pragma once

#include "game_entity_IButtonEntity.h"

#include <bn_optional.h>
#include <bn_sprite_animate_actions.h>

namespace sym::game::entity
{

class PressureButton final : public IButtonEntity
{
public:
    PressureButton(bn::fixed_point position, int textNumber, bool isOnByDefault);

    PressureButton(PressureButton&& other) = delete;
    PressureButton& operator=(PressureButton&& other) = delete;

    PressureButton(const PressureButton& other) = delete;
    PressureButton& operator=(const PressureButton& other) = delete;

    void AllocateGraphicResource(int z_order) final;
    void FreeGraphicResource() final;

    void Update() final;

    void InitButtonOnAction();
    void InitButtonOffAction();

private:
    bn::optional<bn::sprite_animate_action<2>> action_;
};

} // namespace sym::game::entity
