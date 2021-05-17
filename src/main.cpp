#include <bn_bg_palettes.h>
#include <bn_color.h>
#include <bn_core.h>
#include <bn_memory.h>

#include "Constants.h"
#include "SplashScene.h"
#include "bn_assert.h"

int main()
{
    bn::core::init();

    bn::bg_palettes::set_transparent_color(sym::constants::TRANSPARENT_BG_COLOR);
    bn::unique_ptr<sym::Scene> scene(new sym::SplashScene);
    bn::optional<sym::SceneType> nextScene;

    while (true)
    {
        if (scene)
        {
            nextScene = scene->Update();
        }

        bn::core::update();

        if (nextScene)
        {
            switch (*nextScene)
            {
            case sym::SceneType::SPLASH:
                scene.reset(new sym::SplashScene);
                break;
            case sym::SceneType::TITLE:
                // TODO: scene.reset(new sym::TitleScene);
                break;
            // case sym::SceneType::GAME:
            //     break;
            // case sym::SceneType::LICENSE:
            //     break;
            default:
                BN_ERROR("Unknown SceneType: ", (int)*nextScene);
            }
        }
    }
}
