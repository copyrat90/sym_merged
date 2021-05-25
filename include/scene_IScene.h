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

class IScene
{
public:
    virtual ~IScene() = default;
    [[nodiscard]] virtual bn::optional<Type> Update() = 0;

protected:
    IScene() = default;
};

} // namespace sym::scene
