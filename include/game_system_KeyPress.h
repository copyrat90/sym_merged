#pragma once

#include "game_system_ISystem.h"

namespace sym::game::system
{

/**
 * @brief Manages key press lasting for a short period
 *
 */
class KeyPress final : public ISystem
{
public:
    KeyPress(scene::GameState& state);

    void Update() final;

private:
    void UpdateLRKeyLastingCount_();
};

} // namespace sym::game::system
