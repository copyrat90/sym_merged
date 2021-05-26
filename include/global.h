#pragma once

#include <bn_sprite_text_generator.h>
#include <stdint.h>

namespace sym::global
{

namespace setting
{

enum class Lang : uint8_t
{
    ENG,
    KOR,
    LANG_TOTAL_COUNT
};

/**
 * @brief Initialize SRAM save.
 * This must be called first before using any global functions, as it read/writes SRAM save.
 *
 */
void Init();

/**
 * @brief Set the Language used for sprite_text_generator.
 * Also writes language setting to SRAM save.
 *
 */
void SetLang(Lang);
Lang GetLang();

} // namespace setting

/**
 * @brief Get the sprite_text_generator of the current language.
 *
 */
bn::sprite_text_generator* GetTextGen();

bool IsSeenOpening();
void SetSeenOpening();

} // namespace sym::global
