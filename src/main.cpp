#include <bn_core.h>

#include <bn_assert.h>
#include <bn_bg_palettes.h>
#include <bn_color.h>
#include <bn_memory.h>
#include <bn_sprite_text_generator.h>

#ifndef NDEBUG
#include <bn_fixed_point.h>
#include <bn_format.h>
#include <bn_keypad.h>
#include <bn_music.h>
#include <bn_sprite_ptr.h>
#include <bn_string.h>
#include <bn_vector.h>

#endif

#include "constant.h"
#include "global.h"

#include "scene_Game.h"
#include "scene_Param.h"
#include "scene_Splash.h"
#include "scene_Title.h"

using namespace sym;

int main()
{
    bn::core::init();
    sym::global::Init();
    bn::bg_palettes::set_transparent_color(constant::TRANSPARENT_BG_COLOR);

    scene::Param sceneParam;
    // Test
    // sceneParam.SetCurrentStage(sym::global::GetCurrentStage());
    sceneParam.SetCurrentStage(game::stage::Id::W0_S0);
    // bn::unique_ptr<scene::IScene> scene(new scene::Game(sceneParam));
    bn::unique_ptr<scene::IScene> scene(new scene::Splash(sceneParam));
    bn::optional<scene::Type> nextScene;

#ifndef NDEBUG
    constexpr int IWRAM_BYTES = 32'768, EWRAM_BYTES = 262'144;
    int resourceUsageUpdateCountDown = 1;
    bn::vector<bn::sprite_ptr, 24> resourceUsageSprites;
    bool isDebugViewOn = false;
#endif

    while (true)
    {
        if (scene)
        {
            nextScene = scene->Update();
        }

#ifndef NDEBUG
        if (bn::keypad::select_pressed())
        {
            isDebugViewOn = !isDebugViewOn;
            if (!isDebugViewOn)
            {
                resourceUsageSprites.clear();
            }
        }
        if (isDebugViewOn && --resourceUsageUpdateCountDown <= 0)
        {
            resourceUsageSprites.clear();
            auto* const textGen = sym::global::GetTextGen();
            auto prevAlignment = textGen->alignment();
            textGen->set_left_alignment();
            textGen->generate({-120, -70}, bn::format<9>("CPU: {}%", (bn::core::last_cpu_usage() * 100).integer()),
                              resourceUsageSprites);

            const int iwramUsedPercent =
                (bn::fixed(bn::memory::used_static_iwram() + bn::memory::used_stack_iwram()) / IWRAM_BYTES * 100)
                    .integer();
            const int ewramUsedPercent =
                (bn::fixed(EWRAM_BYTES - bn::memory::available_alloc_ewram()) / EWRAM_BYTES * 100).integer();
            const int iwramFree = IWRAM_BYTES - bn::memory::used_static_iwram() - bn::memory::used_stack_iwram();
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
                scene.reset(new scene::Splash(sceneParam));
                break;
            case scene::Type::TITLE:
                scene.reset();
                scene.reset(new scene::Title(sceneParam));
                break;
            case scene::Type::GAME:
                scene.reset();
                scene.reset(new scene::Game(sceneParam));
                break;
            // TODO: Add other scenes
            // case scene::Type::LICENSE:
            //     scene.reset();
            //     scene.reset(new scene::License(sceneParam));
            //     break;
            default:
                BN_ERROR("Unknown SceneType: ", (int)*nextScene);
            }
        }
    }
}
