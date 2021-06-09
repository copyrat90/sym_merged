#pragma once

#include "scene_IScene.h"

#include "game_system_ButtonInteraction.h"
#include "game_system_PlayerMovement.h"
#include "scene_GameState.h"

namespace sym::scene
{
using namespace game;

class Game final : public IScene
{
public:
    Game(scene::Param& sceneParam);
    ~Game();

    [[nodiscard]] bn::optional<Type> Update() final;

private:
    static constexpr int FADE_IN_UPDATE_COUNT = 30;
    static constexpr int FADE_OUT_UPDATE_COUNT = 30;

    /**
     * @brief All member variables reside in here.
     * to easily pass states by reference into Systems.
     *
     */
    GameState state_;

    system::PlayerMovement playerMovement_;
    system::ButtonInteraction buttonInteraction_;

    void SetCurrentZone_(int zoneIdx);
};

} // namespace sym::scene
