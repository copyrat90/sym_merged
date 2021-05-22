#include "global.h"

#include <bn_assert.h>
#include <bn_sprite_text_generator.h>
#include <bn_sram.h>
#include <bn_sstream.h>
#include <bn_string_view.h>

#include "constant.h"

#include "font_galmuri15.h"
#include "font_m6x11.h"

namespace sym::global
{

namespace
{

struct SramSave
{
    constexpr static char MAGIC_BYTES[8] = "SYM000";

    char magicBytes[8];
    setting::Lang currentLang;

    /**
     * @brief Resets some save properties.
     * Language setting is kept intact.
     * This function does NOT ERASE the SRAM save.
     *
     */
    void Init()
    {
        CopyMagicBytes();
    }

    /**
     * @brief Read the save from SRAM.
     *
     * @return `true` if save was read (SRAM save exist)
     * @return `false` if save was failed to read (SRAM save does not exist, or perhaps corrupted)
     */
    [[nodiscard]] bool Read()
    {
        bn::sram::read(*this);
        return bn::string_view(magicBytes) == MAGIC_BYTES;
    }

    void Write()
    {
        CopyMagicBytes();
        bn::sram::write(*this);
    }

    /**
     * @brief Erase the SRAM save.
     *
     */
    void Erase()
    {
        Init();
        Write();
    }

private:
    void CopyMagicBytes()
    {
        bn::istring_base magicBytes_str(magicBytes);
        bn::ostringstream magicBytes_stream(magicBytes_str);
        magicBytes_stream.append(MAGIC_BYTES);
    }
};

SramSave sramSave_;

struct TextGenManager
{
    bn::sprite_text_generator* latin = nullptr;
    bn::sprite_text_generator* hangeul = nullptr;

    void SetLang(setting::Lang lang)
    {
        switch (lang)
        {
        case setting::Lang::ENG:
            if (!latin)
                latin = new bn::sprite_text_generator(font::variable_8x16_m6x11);
            if (hangeul)
                delete hangeul;
            break;
        case setting::Lang::KOR:
            if (!hangeul)
                hangeul = new bn::sprite_text_generator(font::fixed_16x16_galmuri);
            if (latin)
                delete latin;
            break;
        default:
            BN_ERROR("Unknown setting::Lang : ", static_cast<int>(lang));
            break;
        }
    }
};

TextGenManager textGenManager_;

} // anonymous namespace

namespace setting
{

void Init()
{
    if (!sramSave_.Read())
    {
        sramSave_.Init();
        SetLang(sym::constant::DEFAULT_LANG);
    }
    else
    {
        textGenManager_.SetLang(GetLang());
    }
}

void SetLang(Lang lang)
{
    sramSave_.currentLang = lang;
    sramSave_.Write();

    textGenManager_.SetLang(lang);
}

Lang GetLang()
{
    return sramSave_.currentLang;
}

} // namespace setting

bn::sprite_text_generator* GetCurrentLangTextGen()
{
    return GetTextGen(setting::GetLang());
}

bn::sprite_text_generator* GetTextGen(setting::Lang lang)
{
    switch (lang)
    {
    case setting::Lang::ENG:
        return textGenManager_.latin;
    case setting::Lang::KOR:
        return textGenManager_.hangeul;
    default:
        BN_ERROR("Unknown setting::Lang : ", static_cast<int>(lang));
    }
    return nullptr;
}

} // namespace sym::global
