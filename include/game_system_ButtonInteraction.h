#pragma once

#include "game_system_ISystem.h"

namespace sym::game::system
{

class ButtonInteraction final : public ISystem
{
public:
    ButtonInteraction(scene::GameState& state);

    void Update() final;

private:
    void HoverButtonPlayerInteract_();
    void HoverButtonThrownSymbolInteract_();

    void UpdateKeyLastingCount_();
    [[nodiscard]] bool IsLKeyPressLasts_() const;
    [[nodiscard]] bool IsRKeyPressLasts_() const;
    void ResetLKeyPress_();
    void ResetRKeyPress_();
    void ToggleOpenedHoverButtonAssociatedOpenables_(int hoverButtonIdx);
};

} // namespace sym::game::system
