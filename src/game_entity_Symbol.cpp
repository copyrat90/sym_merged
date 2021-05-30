#include "game_entity_Symbol.h"

#include <bn_assert.h>
#include <bn_sprite_tiles_ptr.h>

#include "helper_rect.h"

#include "bn_sprite_items_spr_basic_symbols.h"
#include "bn_sprite_items_spr_complex_symbols.h"

namespace sym::game::entity
{

namespace
{

bool IsComplexSymbol_(Symbol::Type type)
{
    return static_cast<int>(type) >= Symbol::COMPLEX_SYMBOL_START_NUM;
}

bn::fixed_rect GenerateColliderFromSymbolType_(bn::fixed_point position, Symbol::Type symbolType,
                                               bn::sprite_shape_size spriteShapeSize)
{
    const bn::fixed_rect spriteRect(position, {spriteShapeSize.width(), spriteShapeSize.height()});
    const bn::fixed_point topLeft = spriteRect.top_left();

    bn::fixed_point resultTopLeft = topLeft;
    bn::fixed_size resultSize;

    switch (symbolType)
    {
    case Symbol::Type::BAR:
        resultTopLeft += {7, 2};
        resultSize = {2, 12};
        break;
    case Symbol::Type::XOR:
        resultTopLeft += {2, 4};
        resultSize = {12, 7};
        break;
    case Symbol::Type::UP:
        resultTopLeft += {2, 1};
        resultSize = {12, 14};
        break;
    case Symbol::Type::VV:
        resultTopLeft += {0, 2};
        resultSize = {16, 12};
        break;
    case Symbol::Type::PLUS:
        resultTopLeft += {2, 2};
        resultSize = {12, 12};
        break;
    default:
        BN_ERROR("Unknown Symbol::Type : ", static_cast<int>(symbolType));
        break;
    }
    return helper::rect::MakeFixedRectByTopLeftAndSize(resultTopLeft, resultSize);
}

} // namespace

Symbol::Symbol(bn::fixed_point position, Symbol::Type type)
    : Entity(position,
             GenerateColliderFromSymbolType_(position, type,
                                             IsComplexSymbol_(type) ? bn::sprite_items::spr_complex_symbols.shape_size()
                                                                    : bn::sprite_items::spr_basic_symbols.shape_size()),
             IsComplexSymbol_(type) ? &bn::sprite_items::spr_complex_symbols : &bn::sprite_items::spr_basic_symbols),
      type_(type)
{
    AllocateGraphicResource();
}

Symbol::Symbol(Symbol&& other) noexcept : Entity(bn::move(other))
{
}

void Symbol::AllocateGraphicResource()
{
    int spriteIdx = static_cast<int>(type_);
    if (IsComplexSymbol_(type_))
        spriteIdx -= COMPLEX_SYMBOL_START_NUM;

    const bn::sprite_item& spriteItem =
        IsComplexSymbol_(type_) ? bn::sprite_items::spr_complex_symbols : bn::sprite_items::spr_basic_symbols;
    if (!sprite_)
        sprite_ = spriteItem.create_sprite(position_);
    sprite_->set_item(spriteItem);
    sprite_->set_tiles(spriteItem.tiles_item().create_tiles(spriteIdx));
}

Symbol::Type Symbol::GetType() const
{
    return type_;
}

void Symbol::SetType(Symbol::Type newType)
{
    if (type_ != newType)
    {
        type_ = newType;
        AllocateGraphicResource();
    }
}

} // namespace sym::game::entity
