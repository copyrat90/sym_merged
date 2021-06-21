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

    Symbol(bn::fixed_point position, Symbol::Type type);
    Symbol(Symbol&& other);
    Symbol& operator=(Symbol&& other);

    Symbol(const Symbol& other) = delete;
    Symbol& operator=(const Symbol& other) = delete;

    void AllocateGraphicResource(int z_order) final;

    Type GetType() const;
    void SetType(Type);

    bool GetPickedUp() const;
    void SetPickedUp(bool isPickedUp);

    bool GetThrown() const;
    void SetThrown(bool isThrown);

private:
    static constexpr bn::fixed_rect RELATIVE_INTERACT_RANGE = {{0, 0}, {24, 16}};
    static constexpr bool IS_GRAVITY_ENABLED_BY_DEFAULT = true;
    static constexpr bn::fixed GRAVITY_SCALE = 1;

    Symbol::Type type_;
    bool isPickedUp_ = false;
    bool isThrown_ = false;
};

} // namespace sym::game::entity
