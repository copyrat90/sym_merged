#pragma once

#include "game_entity_Entity.h"

namespace sym::game::entity
{

class Symbol final : Entity
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
    Symbol(Symbol&& other) noexcept;

    void AllocateGraphicResource() final;

    Type GetType() const;
    void SetType(Type);

private:
    Symbol::Type type_;
};

} // namespace sym::game::entity
