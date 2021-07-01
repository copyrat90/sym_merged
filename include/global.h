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
 * @brief Set the Language used for sprite_text_generator.
 * Also writes language setting to SRAM save.
 *
 */
void SetLang(Lang);
Lang GetLang();

} // namespace setting

/**
 * @brief Initialize globals
 * This must be called first before using any global functions.
 *
 */
void Init();

/**
 * @brief Get the sprite_text_generator of the current language.
 *
 */
bn::sprite_text_generator* GetTextGen();

unsigned GetRandomNumber();

bool IsSeenOpening();
void SetSeenOpening();

} // namespace sym::global
