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
            if (hangeul)
                delete hangeul;
            if (!latin)
                latin = new bn::sprite_text_generator(font::variable_8x16_m6x11);
            break;
        case setting::Lang::KOR:
            if (latin)
                delete latin;
            if (!hangeul)
                hangeul = new bn::sprite_text_generator(font::fixed_16x16_galmuri);
            break;
        default:
            BN_ERROR("Unknown setting::Lang : ", static_cast<int>(lang));
            break;
        }
    }
};

TextGenManager textGenManager_;

bool isInitialized_ = false;

} // anonymous namespace

namespace setting
{

void Init()
{
    isInitialized_ = true;

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
    BN_ASSERT(isInitialized_, "setting::Init() must be called before using global functions");
    sramSave_.currentLang = lang;
    sramSave_.Write();

    textGenManager_.SetLang(lang);
}

Lang GetLang()
{
    BN_ASSERT(isInitialized_, "setting::Init() must be called before using global functions");
    return sramSave_.currentLang;
}

} // namespace setting

bn::sprite_text_generator* GetCurrentLangTextGen()
{
    BN_ASSERT(isInitialized_, "setting::Init() must be called before using global functions");
    return GetTextGen(setting::GetLang());
}

bn::sprite_text_generator* GetTextGen(setting::Lang lang)
{
    BN_ASSERT(isInitialized_, "setting::Init() must be called before using global functions");
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
