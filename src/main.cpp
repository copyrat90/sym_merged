#include <bn_core.h>

#include <bn_assert.h>
#include <bn_bg_palettes.h>
#include <bn_color.h>
#include <bn_memory.h>
#include <bn_sprite_text_generator.h>

#ifndef NDEBUG
#include <bn_fixed_point.h>
#include <bn_format.h>
#include <bn_sprite_ptr.h>
#include <bn_string.h>
#include <bn_vector.h>

#endif

#include "constant.h"
#include "global.h"

#include "scene_Game.h"
#include "scene_Splash.h"
#include "scene_Title.h"

#include "game_Status.h"

using namespace sym;

int main()
{
    bn::core::init();
    sym::global::setting::Init();
    bn::bg_palettes::set_transparent_color(constant::TRANSPARENT_BG_COLOR);

    game::Status status;
    // Test
    // bn::unique_ptr<scene::IScene> scene(new scene::Splash);
    bn::unique_ptr<scene::IScene> scene(new scene::Game(status));
    bn::optional<scene::Type> nextScene;

#ifndef NDEBUG
    constexpr int IWRAM_BYTES = 32'768, EWRAM_BYTES = 262'144;
    int resourceUsageUpdateCountDown = 1;
    bn::vector<bn::sprite_ptr, 24> resourceUsageSprites;
#endif

    while (true)
    {
        if (scene)
        {
            nextScene = scene->Update();
        }

#ifndef NDEBUG
        if (--resourceUsageUpdateCountDown <= 0)
        {
            resourceUsageSprites.clear();
            auto* const textGen = sym::global::GetTextGen();
            auto prevAlignment = textGen->alignment();
            textGen->set_left_alignment();
            textGen->generate({-120, -70}, bn::format<9>("CPU: {}%", bn::core::last_cpu_usage().integer()),
                              resourceUsageSprites);

            const int iwramUsedPercent = (bn::fixed(bn::memory::used_static_iwram()) / IWRAM_BYTES * 100).integer();
            const int ewramUsedPercent =
                (bn::fixed(EWRAM_BYTES - bn::memory::available_alloc_ewram()) / EWRAM_BYTES * 100).integer();
            const int iwramFree = IWRAM_BYTES - bn::memory::used_static_iwram();
            const int ewramFree = bn::memory::available_alloc_ewram();

            textGen->set_right_alignment();
            textGen->generate({120, -70}, bn::format<20>("IW: {}% {}", iwramUsedPercent, iwramFree),
                              resourceUsageSprites);
            textGen->generate({120, -55},

                              bn::format<20>("EW: {}% {}", ewramUsedPercent, ewramFree), resourceUsageSprites);
            textGen->set_alignment(prevAlignment);
            resourceUsageUpdateCountDown = 5;
        }
#endif

        bn::core::update();

        if (nextScene)
        {
            // scene.reset() must be called twice like below,
            // otherwise 2 Scenes are present in a moment,
            //   : after the new Scene created, but before calling scene.reset(new Scene)
            // which would break the game by:
            //   1. enabling transparency & fading blendings at the same time.
            //   2. setting transparency_action in the new Scene,
            //      and when disposing the old Scene afterward, setting transparency level to normal,
            //      which would break the transparency_action
            switch (*nextScene)
            {
            case scene::Type::SPLASH:
                scene.reset();
                scene.reset(new scene::Splash);
                break;
            case scene::Type::TITLE:
                scene.reset();
                scene.reset(new scene::Title);
                break;
            // TODO: Add other scenes
            // case SceneType::GAME:
            //     scene.reset();
            //     scene.reset(new sym::scene::Game);
            //     break;
            // case SceneType::LICENSE:
            //     scene.reset();
            //     scene.reset(new sym::scene::License);
            //     break;
            default:
                BN_ERROR("Unknown SceneType: ", (int)*nextScene);
            }
        }
    }
}
