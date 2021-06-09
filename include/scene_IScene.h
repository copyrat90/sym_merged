#pragma once

#include <bn_optional.h>

#include "scene_Param.h"
#include "scene_Type.h"

namespace sym::scene
{

class IScene
{
public:
    virtual ~IScene() = default;
    IScene(scene::Param& sceneParam) : sceneParam_(sceneParam)
    {
    }

    [[nodiscard]] virtual bn::optional<Type> Update() = 0;

protected:
    scene::Param& sceneParam_;
};

} // namespace sym::scene
