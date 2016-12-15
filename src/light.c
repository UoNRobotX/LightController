#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>

#include "light.h"
#include "task.h"
#include "e_stop.h"

static void light_task(void*);

typedef enum
{
    MANUAL,
    AUTONOMOUS,
    HOMING
} MODE;

static MODE mode = MANUAL;
static int counter = 0;

void light_init()
{
    // Turn all lights off
    PORTF &= ~((1 << 0) | (1 << 1) | (1 << 2));

    // Set light pins as output
    DDRF |= (1 << 0) | (1 << 1) | (1 << 2);

    static TASK_T task;

    task.func = light_task;
    task.interval = X_ms(100);
    task.priority = 2;
    task.count = -1;
    task_enqueue(&task, NULL);
}

void light_task(void * args)
{
    if (!e_stop_activated())
    {
        if (mode == AUTONOMOUS)
        {
            light_green();
        }
        else if (mode == HOMING)
        {
            counter++;
            if (counter < 5)
            {
                light_red();
            }
            else if (counter < 10)
            {
                light_yellow();
            }
            else if (counter < 15)
            {
                light_green();
            }
            else
            {
                counter = 0;
            }
        }
        else
        {
            light_yellow();
        }
    }
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
                mode = MANUAL;
            }
            else if (!strcmp_P(argv[0], PSTR("green")))
            {
                light_green();
                mode = AUTONOMOUS;
            }
            else if (!strcmp_P(argv[0], PSTR("home")))
            {
                light_red();
                mode = HOMING;
                counter = 0;
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
