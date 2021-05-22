#include "scene_Title.h"

#include <bn_assert.h>
#include <bn_fixed.h>
#include <bn_keypad.h>

#include "bn_regular_bg_items_bg_title.h"
#include "bn_sprite_items_spr_cursor_star.h"

namespace sym::scene
{

Title::Title()
    : cursor_{bn::sprite_items::spr_cursor_star.create_sprite(-CURSOR_HORIZONTAL_OFFSET, 0),
              bn::sprite_items::spr_cursor_star.create_sprite(CURSOR_HORIZONTAL_OFFSET, 0)},
      bg_(bn::regular_bg_items::bg_title.create_bg(0, 0)),
      fadeIn_(effect::Transition::Types::FADE | effect::Transition::Types::SPRITE_MOSAIC |
                  effect::Transition::Types::BG_MOSAIC,
              effect::Transition::Direction::IN, FADE_IN_UPDATE_COUNT),
      fadeOut_(effect::Transition::Types::FADE, effect::Transition::Direction::OUT, FADE_OUT_UPDATE_COUNT)
{
    bg_.set_blending_enabled(true);
    bg_.set_mosaic_enabled(true);
    for (auto& cursor : cursor_)
    {
        cursor.set_blending_enabled(true);
        cursor.set_mosaic_enabled(true);
    }

    fadeIn_.Init();
}

bn::optional<Type> Title::Update()
{
    using Transition = effect::Transition;
    switch (fadeIn_.GetState())
    {
    case Transition::State::NOT_READY:
        BN_ERROR("fadeIn_ somehow not initialized. should not reach here...");
        break;
    case Transition::State::ONGOING:
        fadeIn_.Update();
        // TODO
        break;
    case Transition::State::DONE:
        // TODO
        break;
    default:
        BN_ERROR("Unknown fadeIn_::State : ", (int)fadeIn_.GetState());
        break;
    }

    return bn::nullopt;
}

} // namespace sym::scene
