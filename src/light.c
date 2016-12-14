#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>

#include "light.h"
#include "e_stop.h"

void light_init()
{
    // Turn all lights off
    PORTF &= ~((1 << 0) | (1 << 1) | (1 << 2));

    // Set light pins as output
    DDRF |= (1 << 0) | (1 << 1) | (1 << 2);

    // Default to red light
    light_red();
}

void cmd_light(int argc, char *argv[])
{
    switch (argc)
    {
        case 0:
            puts_P(PSTR("Syntax is \"light [red|yellow|green]\""));
        break;
        default:
            if (!strcmp_P(argv[0], PSTR("red")))
            {
                light_red();
            }
            else if (!strcmp_P(argv[0], PSTR("yellow")))
            {
                light_yellow();
            }
            else if (!strcmp_P(argv[0], PSTR("green")))
            {
                light_green();
            }
            else
            {
                puts_P(PSTR("Valid colours are [red|yellow|green]"));
            }
    }
}

void light_red()
{
    PORTF &= ~((1 << 0) | (1 << 1) | (1 << 2));
    PORTF |= (1 << 2);
}

void light_yellow()
{
    if (e_stop_activated()) return;
    PORTF &= ~((1 << 0) | (1 << 1) | (1 << 2));
    PORTF |= (1 << 1);
}

void light_green()
{
    if (e_stop_activated()) return;
    PORTF &= ~((1 << 0) | (1 << 1) | (1 << 2));
    PORTF |= (1 << 0);
}
