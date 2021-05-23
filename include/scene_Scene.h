#pragma once

#include <bn_optional.h>

namespace sym::scene
{

enum class Type
{
    SPLASH,
    TITLE,
    INTRO,
    SELECT_STAGE,
    STAGE,
    ENDING,
    CREDIT
};

class Scene
{
public:
    virtual ~Scene() = default;
    [[nodiscard]] virtual bn::optional<Type> Update() = 0;

protected:
    Scene() = default;
};

} // namespace sym::scene
