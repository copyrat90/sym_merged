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

constexpr bn::fixed_rect RELATIVE_INTERACT_RANGE = {{0, 0}, {24, 16}};
constexpr bool IS_GRAVITY_ENABLED_BY_DEFAULT = true;
constexpr bn::fixed GRAVITY_SCALE = 0.3;

constexpr int MERGE_START_UPDATE_COUNT = 30;
constexpr int MERGE_END_UPDATE_COUNT = 30;
constexpr int DAMAGE_START_UPDATE_COUNT = 30;
constexpr int DAMAGE_END_UPDATE_COUNT = 30;

bool IsComplexSymbol_(Symbol::Type type)
{
    return static_cast<int>(type) >= Symbol::COMPLEX_SYMBOL_START_NUM;
}

/**
 * @return bn::fixed_rect physics collider relative to the position_.
 */
bn::fixed_rect GenerateColliderFromSymbolType_(Symbol::Type symbolType, const bn::sprite_shape_size& spriteShapeSize)
{
    bn::fixed_point resultTopLeft = {-spriteShapeSize.width() / 2, -spriteShapeSize.height() / 2};
    bn::fixed_size resultSize;

    switch (symbolType)
    {
    case Symbol::Type::BAR:
        resultTopLeft += {6, 1};
        resultSize = {4, 14};
        break;
    case Symbol::Type::XOR:
        resultTopLeft += {1, 3};
        resultSize = {14, 9};
        break;
    case Symbol::Type::UP:
        resultTopLeft += {1, 0};
        resultSize = {14, 16};
        break;
    case Symbol::Type::VV:
        resultTopLeft += {0, 1};
        resultSize = {16, 14};
        break;
    case Symbol::Type::PLUS:
        resultTopLeft += {1, 1};
        resultSize = {14, 14};
        break;
    default:
        BN_ERROR("Unknown Symbol::Type : ", static_cast<int>(symbolType));
        break;
    }
    return helper::rect::MakeFixedRectByTopLeftAndSize(resultTopLeft, resultSize);
}

} // namespace

Symbol::Symbol(bn::fixed_point position, Symbol::Type type)
    : IPhysicsEntity(position, RELATIVE_INTERACT_RANGE,
                     GenerateColliderFromSymbolType_(type, IsComplexSymbol_(type)
                                                               ? bn::sprite_items::spr_complex_symbols.shape_size()
                                                               : bn::sprite_items::spr_basic_symbols.shape_size()),
                     IS_GRAVITY_ENABLED_BY_DEFAULT, GRAVITY_SCALE,
                     IsComplexSymbol_(type) ? &bn::sprite_items::spr_complex_symbols
                                            : &bn::sprite_items::spr_basic_symbols),
      type_(type)
{
}

Symbol::Symbol(Symbol&& other)
    : IPhysicsEntity(bn::move(other)), type_(other.type_), isPickedUp_(other.isPickedUp_), isThrown_(other.isThrown_),
      animationState_(other.animationState_)
{
}

Symbol& Symbol::operator=(Symbol&& other)
{
    IPhysicsEntity::operator=(bn::move(other));
    type_ = other.type_;
    isPickedUp_ = other.isPickedUp_;
    isThrown_ = other.isThrown_;
    animationState_ = other.animationState_;
    return *this;
}

void Symbol::AllocateGraphicResource(int z_order)
{
    int spriteIdx = static_cast<int>(type_);
    if (IsComplexSymbol_(type_))
        spriteIdx -= COMPLEX_SYMBOL_START_NUM;
    spriteItem_ =
        IsComplexSymbol_(type_) ? &bn::sprite_items::spr_complex_symbols : &bn::sprite_items::spr_basic_symbols;

    sprite_ = spriteItem_->create_sprite(position_, spriteIdx);
    sprite_->set_z_order(z_order);
    sprite_->set_blending_enabled(isBlendingEnabled_);
    sprite_->set_mosaic_enabled(isMosaicEnabled_);
    sprite_->set_visible(isVisible_);
    if (paletteItem_)
        SetColors(*paletteItem_);
}

void Symbol::Update()
{
    IPhysicsEntity::Update();

    UpdateAnimation_();
}

void Symbol::InitMergeStartAction()
{
    BN_ASSERT(sprite_, "Symbol action cannot be init without allocating graphics!");
    DestroyAnimation_();
    animationState_ = AnimationState::MERGE_START;
    // TODO
}

void Symbol::InitMergeEndAction()
{
    BN_ASSERT(sprite_, "Symbol action cannot be init without allocating graphics!");
    DestroyAnimation_();
    animationState_ = AnimationState::MERGE_END;
    // TODO
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
        if (sprite_)
            AllocateGraphicResource(sprite_->z_order());
    }
}

bool Symbol::GetPickedUp() const
{
    return isPickedUp_;
}

void Symbol::SetPickedUp(bool isPickedUp)
{
    isPickedUp_ = isPickedUp;
}

bool Symbol::GetThrown() const
{
    return isThrown_;
}

void Symbol::SetThrown(bool isThrown)
{
    isThrown_ = isThrown;
}

Symbol::AnimationState Symbol::GetAnimationState() const
{
    return animationState_;
}

bool Symbol::GetAnimationDone() const
{
    // TODO
    return true;
}

bool Symbol::UpdateAnimation_()
{
    // TODO
    return true;
}

void Symbol::DestroyAnimation_()
{
    // TODO
}

} // namespace sym::game::entity
