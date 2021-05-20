#include "helper_keypad.h"

namespace sym::helper::keypad
{

bn::keypad::key_type operator|(bn::keypad::key_type a, bn::keypad::key_type b)
{
    return static_cast<bn::keypad::key_type>(static_cast<uint16_t>(a) | static_cast<uint16_t>(b));
}

} // namespace sym::helper::keypad
