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
    static constexpr int KEYPRESS_LASTING_UPDATE_COUNT = 5;
    int lKeyLastingCount = -1;
    int rKeyLastingCount = -1;

    void HoverButtonPlayerInteract_();
    void HoverButtonThrownSymbolInteract_();

    void UpdateKeyLastingCount_();
    bool IsLKeyPressLasts_() const;
    bool IsRKeyPressLasts_() const;
    void ResetLKeyPress_();
    void ResetRKeyPress_();
    void ToggleOpenedHoverButtonAssociatedOpenables_(int hoverButtonIdx);
};

} // namespace sym::game::system
