#pragma once

#include <bn_keypad.h>

namespace sym::helper::keypad
{

bn::keypad::key_type operator|(bn::keypad::key_type a, bn::keypad::key_type b);

} // namespace sym::helper::keypad