#pragma once

#include "scene_IScene.h"

#include <bn_array.h>
#include <bn_music_actions.h>
#include <bn_optional.h>
#include <bn_sprite_ptr.h>
#include <bn_vector.h>

#include "effect_Transition.h"

namespace sym::scene
{

class DemoEnd final : public IScene
{
public:
    DemoEnd(scene::Param& sceneParam);
    ~DemoEnd();

    [[nodiscard]] virtual bn::optional<Type> Update() final;

private:
    bn::array<bn::vector<bn::sprite_ptr, 8>, 5> textSprites_;
    bn::optional<bn::music_volume_to_action> volumeFadeOut_;
    bn::optional<scene::Type> reservedSceneType_;
    int waitTillGetInput_ = 60;

    effect::Transition fadeIn_;
    effect::Transition fadeOut_;
};

} // namespace sym::scene
