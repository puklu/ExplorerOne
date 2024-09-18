#include "io.hpp"

#define IO_PIN_MASK (0x0F)
#define IO_PORT_OFFSET (4u)
#define IO_PORT_MASK (0x70)

namespace IO
{
uint8_t get_port_number(IO::pin pin_name)
{
    return (pin_name & IO_PORT_MASK) >> IO_PORT_OFFSET;
}

uint8_t get_pin_number(IO::pin pin_name)
{
    return pin_name & IO_PIN_MASK;
}

void enable(IO::pin pin_name)
{
    // TODO: Implement
    (void)pin_name;
}

void set_direction(IO::pin pin_name, IO::direction dir)
{
    // TODO : Implement
    (void)pin_name;
    (void)dir;
}

void set_resistor(IO::pin pin_name, IO::pupd_resistor resistor)
{
    // TODO : Implement
    (void)pin_name;
    (void)resistor;
}

}  // namespace IO