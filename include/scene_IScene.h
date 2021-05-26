#pragma once

#include <bn_optional.h>

#include "scene_Type.h"

namespace sym::scene
{

class IScene
{
public:
    virtual ~IScene() = default;
    [[nodiscard]] virtual bn::optional<Type> Update() = 0;

protected:
    IScene() = default;
};

} // namespace sym::scene
