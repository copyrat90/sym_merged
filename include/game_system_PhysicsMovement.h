#pragma once

#include "game_system_ISystem.h"

namespace sym::game::system
{

/**
 * @brief Physics Update, along with sound effects.
 * Only deals with colliders, not triggers.
 * (e.g. no pressure button & door interations)
 *
 */
class PhysicsMovement final : public ISystem
{
public:
    PhysicsMovement(scene::GameState& state);

    void Update() final;

private:
    void UpdatePlayer_();
    void PlayerKeyboardHandle_();
    void PlayerCollision_();
    void PlayerAnimation_();

    void UpdateSymbols_();
    void UpdateSymbolsInHands_();
    void UpdateSymbolsOfCurrentZone_();
    void UpdateSymbolsThrown_();
    void SymbolCollision_(entity::Symbol&);

    static constexpr int MAX_PLAYER_JUMP_COUNT = 1;
    int playerJumpCount = MAX_PLAYER_JUMP_COUNT;
};

} // namespace sym::game::system
