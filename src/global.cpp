#include "global.h"

#include <bn_assert.h>
#include <bn_random.h>
#include <bn_sprite_text_generator.h>
#include <bn_sram.h>
#include <bn_sstream.h>
#include <bn_string_view.h>
#include <cstdint>

#include "constant.h"

#include "font_hamchorom.h"
#include "font_m6x11.h"

namespace sym::global
{

namespace
{

struct SramSave
{
    constexpr static char MAGIC_BYTES[8] = "SYM001";

    char magicBytes[8];
    setting::Lang currentLang;

    /**
     * @brief Stores where you are now.
     *
     */
    game::stage::Id currentStageId;

    /**
     * @brief Miscellaneous game flags.
     *
     * @details from LSB to MSB
     * bit 0 : already seen opening
     *     1 : (reserved)
     *
     */
    uint32_t flags;

    /**
     * @brief Stores the cleared stage by flags.
     * stored by `= (1 << game::stage::Id)`.
     *
     */
    uint64_t clearedStages;

    /**
     * @brief Resets some save properties.
     * Language setting is kept intact.
     * This function does NOT ERASE the SRAM save.
     *
     */
    void Init()
    {
        CopyMagicBytes();
        currentLang = setting::Lang::ENG;
        currentStageId = game::stage::Id::W1_S0;
        flags = 0;
        clearedStages = 0;
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
            {
                delete hangeul;
                hangeul = nullptr;
            }
            if (!latin)
            {
                latin = new bn::sprite_text_generator(font::variable_8x16_m6x11);
                latin->set_alignment(constant::DEFAULT_TEXT_GEN_ALIGN);
            }
            break;
        case setting::Lang::KOR:
            if (latin)
            {
                delete latin;
                latin = nullptr;
            }
            if (!hangeul)
            {
                hangeul = new bn::sprite_text_generator(font::fixed_16x16_hamchorom);
                hangeul->set_alignment(constant::DEFAULT_TEXT_GEN_ALIGN);
            }
            break;
        default:
            BN_ERROR("Unknown setting::Lang : ", static_cast<int>(lang));
            break;
        }
    }
};

TextGenManager textGenManager_;

bool isInitialized_ = false;

/**
 * @brief Initialize SRAM save.
 *
 */
void InitSram_()
{
    if (!sramSave_.Read())
    {
        sramSave_.Init();
        SetLang(sym::constant::DEFAULT_LANG);
    }
    else
    {
        textGenManager_.SetLang(setting::GetLang());
    }
}

bn::random* rng;

void InitRNG_()
{
    rng = new bn::random;
}

} // anonymous namespace

namespace setting
{

void SetLang(Lang lang)
{
    BN_ASSERT(isInitialized_, "global::Init() must be called before using global functions");
    sramSave_.currentLang = lang;
    sramSave_.Write();

    textGenManager_.SetLang(lang);
}

Lang GetLang()
{
    BN_ASSERT(isInitialized_, "global::Init() must be called before using global functions");
    return sramSave_.currentLang;
}

} // namespace setting

void Init()
{
    isInitialized_ = true;
    InitSram_();
    InitRNG_();
}

bn::sprite_text_generator* GetTextGen()
{
    BN_ASSERT(isInitialized_, "global::Init() must be called before using global functions");
    switch (setting::GetLang())
    {
    case setting::Lang::ENG:
        return textGenManager_.latin;
    case setting::Lang::KOR:
        return textGenManager_.hangeul;
    default:
        BN_ERROR("Unknown setting::Lang : ", static_cast<int>(setting::GetLang()));
    }
    return nullptr;
}

unsigned GetRandomNumber()
{
    BN_ASSERT(isInitialized_, "global::Init() must be called before using global functions");
    return rng->get();
}

bool GetSeenOpening()
{
    return sramSave_.flags & (1 << 0);
}

void SetSeenOpening()
{
    sramSave_.flags |= (1 << 0);
    sramSave_.Write();
}

game::stage::Id GetCurrentStage()
{
    return sramSave_.currentStageId;
}

void SetCurrentStage(game::stage::Id stage)
{
    sramSave_.currentStageId = stage;
    sramSave_.Write();
}

bool GetStageCleared(game::stage::Id stage)
{
    return (sramSave_.clearedStages & (1 << static_cast<int>(stage)));
}

void SetStageCleared(game::stage::Id stage)
{
    sramSave_.clearedStages |= (1 << static_cast<int>(stage));
    sramSave_.Write();
}

} // namespace sym::global
