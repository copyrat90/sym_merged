#pragma once

#include "game_system_ISystem.h"

#include <bn_fixed.h>
#include <bn_sprite_ptr.h>
#include <bn_vector.h>

namespace sym::game::system
{

class StageNameSplash final : public ISystem
{
public:
    StageNameSplash(scene::GameState& state);

    void Update() final;

    void Destroy();
    void RedrawSprites();

    static constexpr int SENTENCE_SPRITE_SIZE = 8;

private:
    enum class State
    {
        NOT_STARTED,
        SHOW_UP,
        WAIT_CENTER,
        FLY_AWAY,
        DONE
    };

    State nameState_ = State::SHOW_UP;
    State subNameState_ = State::NOT_STARTED;
    int nameCountDown_;
    int subNameCountDown_;
    bn::fixed namePosX_;
    bn::fixed subNamePosX_;

    bn::vector<bn::sprite_ptr, SENTENCE_SPRITE_SIZE> nameSprites_;
    bn::vector<bn::sprite_ptr, SENTENCE_SPRITE_SIZE> subNameSprites_;
};

} // namespace sym::game::system
