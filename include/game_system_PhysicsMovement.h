#pragma once

#include "game_system_ISystem.h"

#include "game_entity_Symbol.h"

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
    bool pressAJumping = false;
    bool symbolJumping = false;

    void UpdatePlayer_();
    void PlayerKeyboardHandle_();
    void PlayerCollision_();
    void PlayerAnimation_();

    void UpdateSymbols_();
    void UpdateSymbolsInHands_();
    void UpdateSymbolsOfCurrentZone_();
    void UpdateSymbolsThrown_();
    void SymbolCollision_(entity::Symbol&);

    void SetAbilityStateOfCertainTypeSymbols_(entity::Symbol::Type type, entity::Symbol::AbilityState);
};

} // namespace sym::game::system
