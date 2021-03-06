#pragma once

#include "scene_IScene.h"

#include "game_system_KeyPress.h"
#include "game_system_PhysicsMovement.h"
#include "game_system_TriggerInteraction.h"
#include "game_system_ZoneSwitch.h"
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
    /**
     * @brief All member variables reside in here.
     * to easily pass states by reference into Systems.
     *
     */
    GameState state_;

    void SetCurrentZone_(int zoneIdx);
};

} // namespace sym::scene
