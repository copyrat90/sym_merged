#include <bn_core.h>

#include <bn_assert.h>
#include <bn_bg_palettes.h>
#include <bn_color.h>
#include <bn_memory.h>
#include <bn_sprite_text_generator.h>

#ifndef NDEBUG
#include <bn_fixed_point.h>
#include <bn_sprite_ptr.h>
#include <bn_string.h>
#include <bn_vector.h>

#endif

#include "constant.h"
#include "global.h"

#include "font_galmuri15.h"
#include "font_m6x11.h"
#include "scene_Splash.h"
#include "scene_Title.h"

using namespace sym;

int main()
{
    bn::core::init();
    bn::bg_palettes::set_transparent_color(constant::TRANSPARENT_BG_COLOR);

    bn::unique_ptr<scene::Scene> scene(new scene::Splash);
    bn::optional<scene::Type> nextScene;

    global::textGenPtr_g = new global::TextGen(new bn::sprite_text_generator(font::fixed_16x16_galmuri),
                                               new bn::sprite_text_generator(font::variable_8x16_m6x11));
#ifndef NDEBUG
    int cpuUsageUpdateCountDown = 1;
    bn::vector<bn::sprite_ptr, 4> cpuUsageSprites;
#endif

    while (true)
    {
        if (scene)
        {
            nextScene = scene->Update();
        }

#ifndef NDEBUG
        if (--cpuUsageUpdateCountDown <= 0)
        {
            cpuUsageSprites.clear();
            auto* latin = global::textGenPtr_g->latin;
            auto prevAlignment = latin->alignment();
            latin->set_right_alignment();
            latin->generate({120, -70}, bn::to_string<15>(bn::core::last_cpu_usage()) + "%", cpuUsageSprites);
            latin->set_alignment(prevAlignment);
            cpuUsageUpdateCountDown = 5;
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
