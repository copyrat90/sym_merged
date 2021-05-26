#pragma once

#include "scene_IScene.h"

#include "global.h"

#include <bn_array.h>
#include <bn_fixed_point.h>
#include <bn_optional.h>
#include <bn_regular_bg_ptr.h>
#include <bn_sprite_ptr.h>
#include <bn_string_view.h>
#include <bn_vector.h>

#include "effect_Transition.h"

namespace sym::scene
{

class Title final : public IScene
{
public:
    Title();
    ~Title() = default;
    [[nodiscard]] bn::optional<Type> Update() final;

private:
    enum class MenuOption
    {
        START,
        LANGUAGE,
        CREDIT,
        MENU_OPTION_TOTAL_COUNT
    };
    static constexpr int MENU_OPTION_TOTAL_COUNT = static_cast<int>(MenuOption::MENU_OPTION_TOTAL_COUNT);
    static constexpr int LANG_TOTAL_COUNT = static_cast<int>(global::setting::Lang::LANG_TOTAL_COUNT);

    bn::array<bn::sprite_ptr, 2> cursor_;
    bn::regular_bg_ptr bg_;
    bn::array<bn::vector<bn::sprite_ptr, 4>, MENU_OPTION_TOTAL_COUNT> menuTextSprites_;
    effect::Transition fadeIn_;
    effect::Transition fadeOut_;

    static constexpr bn::string_view MENU_STRINGS[LANG_TOTAL_COUNT][MENU_OPTION_TOTAL_COUNT] = {
        {"Start", "언 어 : English", "Credit"}, {"시작", "뷁웱:한국어", "크레딧"}};
    static constexpr bn::fixed_point MENU_STRING_POS[LANG_TOTAL_COUNT][MENU_OPTION_TOTAL_COUNT] = {
        {{0, 0}, {0, 20}, {0, 40}}, {{0, 0}, {0, 20}, {0, 40}}};
    static constexpr bn::fixed_point CURSOR_OFFSET[LANG_TOTAL_COUNT] = {{-13, -2}, {-13, -1}};
    static constexpr int FADE_IN_UPDATE_COUNT = 30;
    static constexpr int FADE_OUT_UPDATE_COUNT = 30;

    MenuOption cursorPointingOption_ = MenuOption::START;
    bn::optional<scene::Type> reservedNextScene_ = bn::nullopt;

    /**
     * @brief Handle Up or Down press by moving cursorPointingOption_
     *
     */
    void HandleUpDownPress_();

    /**
     * @brief Handle Start or A press by setting reservedNextScene_
     *
     */
    void HandleStartAPress_();

    /**
     * @brief Advance cursorPointingOption_
     *
     * can advance backwards.
     */
    void AdvanceCursorPointingOption_(int amount);

    void UpdateCursorSpritePosition_();

    /**
     * @brief Clears and redraws menu text sprites.
     * Also sets the blending of the sprites enabled.
     *
     */
    void RedrawMenuTextSprites_();
};

} // namespace sym::scene
