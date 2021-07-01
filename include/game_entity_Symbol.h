#pragma once

#include "game_entity_IPhysicsEntity.h"

namespace sym::game::entity
{

class Symbol final : public IPhysicsEntity
{
public:
    static constexpr int COMPLEX_SYMBOL_START_NUM = 100;
    /**
     * @brief Symbol type
     *
     * If `[enum value] >= COMPLEX_SYMBOL_START_NUM`, it is a complex symbol.
     * otherwise, it is a basic symbol.
     */
    enum Type
    {
        BAR,
        XOR,
        UP = COMPLEX_SYMBOL_START_NUM,
        VV,
        PLUS
    };

    enum class AbilityState
    {
        UNAVAILABLE,
        READY_TO_USE,
        USING,
        NOT_READY
    };

    Symbol(bn::fixed_point position, Symbol::Type type, bool isGravityReversedByDefault);
    Symbol(Symbol&& other);
    Symbol& operator=(Symbol&& other);

    Symbol(const Symbol& other) = delete;
    Symbol& operator=(const Symbol& other) = delete;

    void AllocateGraphicResource(int z_order) final;

    void Update() final;

    [[nodiscard]] Type GetType() const;
    // void SetType(Type);

    void SetGravityReversed(bool isGravityReversed) final;
    [[maybe_unused]] bool ToggleGravityReversed() final;

    [[nodiscard]] AbilityState GetAbilityState() const;
    void SetAbilityState(AbilityState);

    [[nodiscard]] bool GetPickedUp() const;
    void SetPickedUp(bool isPickedUp);

    [[nodiscard]] bool GetThrown() const;
    void SetThrown(bool isThrown);

    [[nodiscard]] bool IsComplexSymbol() const;

private:
    Symbol::Type type_;
    AbilityState abilityState_;
    bool isPickedUp_ = false;
    bool isThrown_ = false;
};

} // namespace sym::game::entity
