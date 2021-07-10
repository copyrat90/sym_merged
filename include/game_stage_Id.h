#pragma once

#include <cstdint>

namespace sym::game::stage
{

/**
 * @brief Stage ID.
 * 'W' stands for "World", 'S' stands for "Stage".
 * The value of the enum is used on saving cleared stages.
 * DO NOT CHANGE THE ORDER, OR THE CLEARED STAGES WILL BE MIXED!
 *
 */
enum class Id : uint8_t
{
    W0_S0, // test map
    W1_S0, // tutorial
    W1_S1,
    W1_S2,
    STAGE_ID_TOTAL_COUNT
};

} // namespace sym::game::stage
