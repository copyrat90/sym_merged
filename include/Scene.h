#pragma once

#include <bn_optional.h>

namespace sym
{

enum class SceneType
{
    SPLASH,
    TITLE,
    GAME,
    LICENSE
};

class Scene
{
public:
    virtual ~Scene() = default;
    [[nodiscard]] virtual bn::optional<SceneType> Update() = 0;

protected:
    Scene() = default;
};

} // namespace sym
