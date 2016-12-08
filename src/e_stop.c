#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>

#include "e_stop.h"
#include "light.h"
#include "task.h"

static void e_stop_task(void*);

void e_stop_init()
{
    // Set e-stop read pin to input
    DDRF &= ~(1 << 3);

    static TASK_T e_stop;

    e_stop.func = e_stop_task;
    e_stop.interval = X_ms(100);
    e_stop.priority = 2;
    e_stop.count = -1;
    task_enqueue(&e_stop, NULL);
}

void e_stop_task(void *args)
{
    if (PINF & (1 << 3))
    {
        puts_P(PSTR("E-Stop OFF"));
    }
    else
    {
        puts_P(PSTR("E-Stop ON"));
        light_red();
    }
}
