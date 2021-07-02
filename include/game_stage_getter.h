#pragma once

#include <bn_span.h>

#include "game_stage_StageInfo.h"

// 'W' stands for "World", 'S' stands for "Stage"
namespace sym::game::stage
{
const StageInfo& Get_W0_S0(); // test stage
const StageInfo& Get_W1_S0(); // tutorial

} // namespace sym::game::stage
