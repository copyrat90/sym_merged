#pragma once

#include <bn_bgs_mosaic_actions.h>
#include <bn_blending_actions.h>
#include <bn_optional.h>
#include <bn_sprites_mosaic_actions.h>

namespace sym::effect
{

/**
 * @brief Simple end-to-end transition
 * Manages all the actions including transparency, fade, intensity, and mosaic.
 *
 */
class Transition
{
public:
    /**
     * @brief Transition Type.
     * Keep in mind that FADE and other blendings cannot be enabled at the same time.
     * (but MOSAIC is not a blending, so it can be used with FADE without any issue.)
     *
     */
    enum class Types
    {
        NONE = 0,
        TRANSPARENCY = 1,
        FADE = 2,
        INTENSITY = 4,
        SPRITE_MOSAIC = 8,
        BG_MOSAIC = 16
    };
    enum class Direction
    {
        NONE,
        IN,
        OUT
    };
    enum class State
    {
        NOT_READY,
        ONGOING,
        DONE
    };

    Transition(Types types, Direction direction, int updateCount);
    Transition();

    ~Transition();

    /**
     * @brief Allocates transition actions & sets state to ONGOING.
     * You must call this function before using Transition.
     *
     * Calling Init() of the second transition object after calling Destroy() of the first transition object,
     * you can prevent having FADE and other blendings at the same time,
     * while keeping both Transition objects alive.
     *
     */
    void Init();

    /**
     * @brief Allocates transition actions & sets state to ONGOING.
     * You must call this function before using Transition.
     *
     * Calling Init() of the second transition object after calling Destroy() of the first transition object,
     * you can prevent having FADE and other blendings at the same time,
     * while keeping both Transition objects alive.
     *
     */
    void Init(Types types, Direction direction, int updateCount);

    /**
     * @brief Updates transition actions.
     * you must check Done() first.
     * Because updating the finished action will break the game.
     * When transition is done, this function automatically calls Destroy() and sets state to DONE.
     *
     */
    void Update();

    /**
     * @brief Disposes transition actions.
     * This function is automatically called when the transition is done.
     * Keep in mind that you have to manually reset the blending alpha.
     *
     */
    void Destroy();

    /**
     * @brief Get the State object
     *
     * @return current state
     */
    State GetState() const
    {
        return state_;
    }

    /**
     * @brief Mimics constructing a new effect::Transition object.
     * You can't use this if transition is ongoing.
     *
     */
    void Set(Types types, Direction direction, int updateCount);

    /**
     * @brief Get the transition types.
     *
     */
    Types GetTypes() const;

    /**
     * @brief Set the transition types.
     * You can't set the transition if it is ongoing.
     *
     */
    void SetTypes(Types types);

    Direction GetDirection() const;

    /**
     * @brief Set the direction
     * You can't set it if it is ongoing.
     *
     */
    void SetDirection(Direction);

    /**
     * @brief Get the update count, which is used to set the blending actions.
     *
     */
    int GetUpdateCount() const;

    /**
     * @brief Set the update count
     * You can't set it if it is ongoing.
     *
     */
    void SetUpdateCount(int updateCount);

    /**
     * @brief Get the remaining update count before it is done.
     *
     */
    int GetRemainingUpdateCount() const;

private:
    Types types_;
    Direction direction_;
    int updateCount_;
    int updateCountDown_;

    State state_ = State::NOT_READY;

    bn::optional<bn::blending_transparency_alpha_to_action> transparencyAction_;
    bn::optional<bn::blending_fade_alpha_to_action> fadeAction_;
    bn::optional<bn::blending_intensity_alpha_to_action> intensityAction_;
    bn::optional<bn::sprites_mosaic_stretch_to_action> spriteMosaicAction_;
    bn::optional<bn::bgs_mosaic_stretch_to_action> bgMosaicAction_;
};

[[nodiscard]] constexpr Transition::Types operator|(Transition::Types t1, Transition::Types t2)
{
    return static_cast<Transition::Types>(static_cast<int>(t1) | static_cast<int>(t2));
}

[[nodiscard]] constexpr Transition::Types operator&(Transition::Types t1, Transition::Types t2)
{
    return static_cast<Transition::Types>(static_cast<int>(t1) & static_cast<int>(t2));
}

} // namespace sym::effect
