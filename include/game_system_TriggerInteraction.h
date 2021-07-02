#pragma once

#include "game_system_ISystem.h"

#include <bn_list.h>
#include <bn_utility.h>

#include "game_entity_HoverButton.h"
#include "game_entity_Sign.h"
#include "game_entity_Symbol.h"

namespace sym::game::system
{

/**
 * @brief Manages trigger interactions, but not colliders.
 * (e.g. symbol pick-up & put-down, button toggle).
 * Switching zones stuff (doors and exits) are dealt in `system::ZoneSwitch` instead.
 *
 */
class TriggerInteraction final : public ISystem
{
public:
    TriggerInteraction(scene::GameState& state);

    void Update() final;

    [[nodiscard]] bool IsLKeyPressLasts() const;
    [[nodiscard]] bool IsRKeyPressLasts() const;
    void ResetLKeyPress();
    void ResetRKeyPress();

private:
    enum class EntityType
    {
        NONE,
        SYMBOL,
        HOVER_BUTTON
    };
    enum class Hand
    {
        LEFT,
        RIGHT
    };

    entity::Sign* collidedSign_ = nullptr;
    bn::vector<bn::sprite_ptr, 20> tooltipTextSprites_;

    [[nodiscard]] bn::pair<EntityType, Hand> GetNearestInteractableFromPlayer_(
        bn::ilist<entity::Symbol>::iterator& outSymbolIter, int& outHoverButtonIdx);

    void PlayerPicksUpSymbol_(Hand interactHand, bn::ilist<entity::Symbol>::iterator symbolIter);
    void PlayerPutsDownSymbol_(Hand hand);
    void PlayerClicksHoverButton_(int hoverButtonIdx);
    void InteractHoverButtonsAndThrownSymbol_();
    void InteractPressureButtonsAndEntities_();

    void UpdateSignTooltipMessages_();
    void UpdateKeyLastingCount_();
    void ToggleOpenedHoverButtonAssociatedOpenables_(int hoverButtonIdx);
    void ToggleOpenedPressureButtonAssociatedOpenables_(int pressureButtonIdx);
};

} // namespace sym::game::system
