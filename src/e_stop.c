#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>

#include "e_stop.h"
#include "light.h"
#include "task.h"

static void e_stop_task(void*);

void e_stop_init()
{
    // Setup ADC
    ADCSRA = (1 << ADEN) | (3 << ADPS0);

    static TASK_T e_stop;

    e_stop.func = e_stop_task;
    e_stop.interval = X_ms(100);
    e_stop.priority = 2;
    e_stop.count = -1;
    task_enqueue(&e_stop, NULL);
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
        puts_P(PSTR("E-Stop OFF"));
        light_yellow();
    }
    else
    {
        puts_P(PSTR("E-Stop ON"));
        light_red();
    }
}
