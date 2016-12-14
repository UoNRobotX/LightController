#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>

#include "e_stop.h"
#include "light.h"
#include "task.h"

static bool e_stop;
static void e_stop_task(void*);

void e_stop_init()
{
    // Setup ADC
    ADCSRA = (1 << ADEN) | (3 << ADPS0);

    static TASK_T task;

    task.func = e_stop_task;
    task.interval = X_ms(100);
    task.priority = 2;
    task.count = -1;
    task_enqueue(&task, NULL);

    // Assume e_stop on
    e_stop = true;
}

void e_stop_task(void *args)
{
    // Start conversion
    ADMUX = (1 << ADLAR) | (1 << REFS0) | (3 << MUX0);
    ADCSRA |= (1 << ADSC);

    while(ADCSRA & (1 << ADSC));
    uint8_t result = ADCH;

    if (result > 150)
    {
        e_stop = false;
        puts_P(PSTR("E-Stop OFF"));
        light_yellow();
    }
    else
    {
        e_stop = true;
        puts_P(PSTR("E-Stop ON"));
        light_red();
    }
}

bool e_stop_activated()
{
    return e_stop;
}
