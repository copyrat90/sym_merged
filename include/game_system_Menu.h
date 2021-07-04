#pragma once

#include "game_system_ISystem.h"

#include <bn_array.h>
#include <bn_sprite_ptr.h>
#include <bn_vector.h>

namespace sym::game::system
{

class Menu final : public ISystem
{
public:
    Menu(scene::GameState& state);

    void Update() final;

    enum class MenuOption
    {
        CONTINUE,
        RESTART_ZONE,
        RESTART_STAGE,
        CHANGE_LANGUAGE,
        // BACK_TO_STAGE_SELECT,
        MENU_OPTION_TOTAL_COUNT
    };
    static constexpr int MENU_OPTION_TOTAL_COUNT = static_cast<int>(Menu::MenuOption::MENU_OPTION_TOTAL_COUNT);

private:
    MenuOption cursorPointingOption_ = MenuOption::CONTINUE;
    bn::array<bn::sprite_ptr, 2> cursorSprites_;
    bn::array<bn::vector<bn::sprite_ptr, 8>, 2> stageNameSprites_;
    bn::array<bn::vector<bn::sprite_ptr, 5>, MENU_OPTION_TOTAL_COUNT> menuOptionSprites_;

    void AdvanceCursorPointingOption_(int amount);

    void SetVisible_(bool isVisible);
    void SetBlendingAndMosaic_(bool isEnabled);
    void RedrawMenuTextSprites_();
    void UpdateCursorSpritePosition_();

    void TogglePause_();
    void RestartZone_();
    void RestartStage_();
    void ChangeLanguage_();
    void GoBackToStageSelect_();
};

} // namespace sym::game::system
