#pragma once

#include "game_system_ISystem.h"

namespace sym::game::system
{

class PhysicsMovement final : public ISystem
{
public:
    PhysicsMovement(scene::GameState& state);

    void Update() final;

private:
    void UpdatePlayer_();
    void PlayerKeyboardHandle_();
    void PlayerGravity_();
    void PlayerClampVelocity_();
    void PlayerCollision_();
    void PlayerAnimation_();

    void UpdateSymbols_();
    void UpdateSymbolsInHands_();
    void UpdateSymbolsOnFloor_();
    void UpdateSymbolsThrown_();

    static constexpr int MAX_PLAYER_JUMP_COUNT = 1;
    int playerJumpCount = MAX_PLAYER_JUMP_COUNT;
};

} // namespace sym::game::system
