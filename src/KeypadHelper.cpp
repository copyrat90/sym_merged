#include "KeypadHelper.h"

namespace sym::helper
{

bn::keypad::key_type operator|(bn::keypad::key_type a, bn::keypad::key_type b)
{
    return bn::keypad::key_type((uint16_t)a | (uint16_t)b);
}

} // namespace sym::helper
