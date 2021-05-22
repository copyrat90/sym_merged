#pragma once

#include <bn_sprite_text_generator.h>

namespace sym::global
{

namespace setting
{

enum class Lang
{
    ENG,
    KOR
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
bn::sprite_text_generator* GetCurrentLangTextGen();

/**
 * @brief Get the sprite_text_generator of the current language.
 *
 */
inline bn::sprite_text_generator* GetTextGen()
{
    return GetCurrentLangTextGen();
}

/**
 * @brief Get the sprite_text_generator of the specified language.
 *
 */
bn::sprite_text_generator* GetTextGen(setting::Lang);

} // namespace sym::global
