#include <avr/interrupt.h>
#include <avr/io.h>
#include <stddef.h>
#include <avr/pgmspace.h>
#include <stdio.h>

#include "task.h"

static volatile int16_t task_count = 0;

static void task_execute(TASK_T *exec);
static TASK_T *task_start = NULL;

ISR (TIMER0_COMPA_vect)
{
	task_count++;
}

void task_init (void)
{
	// Initialise Timer 0
	TCCR0A = (1 << WGM01);										// CTC timer mode
	TCCR0B = (3 << CS00);										// Prescaler of 64
	TIMSK0 |= (1 << OCIE0A);									// Enable CTC interrupt
	
	OCR0A = 125;												// Compare match every 0.5ms
}

void task_yield (void)
{
    uint8_t highestPriority = 0;
    TASK_T* exec = NULL;

    for (TASK_T *current = task_start; current != NULL; current = current->next)
	{
        // Loop through all tasks
		if ((int16_t) (task_count - current->time) >= 0)
        {
            uint8_t priority = current->priority + task_count - current->time;

            if (current->count && (priority >= highestPriority))
            {
                exec = current;
                highestPriority = priority;
            }
		}
    }

    task_execute(exec);
}

void task_execute(TASK_T* exec)
{
    // If the task is scheduled to execute
    if (exec != NULL)
    {
        // Set the conditions for the next call
        exec->time = task_count + exec->interval;

        // Save scheduler information
        int16_t count = exec->count;
        exec->count = 0;

        // Execute task
        exec->func(exec->args);

        // Restore scheduler information
        exec->count = count;

        // Update task counter
        if (exec->count > 0)
            exec->count--;
    }
}

void task_enqueue(TASK_T *task, void* args)
{
    task->args = args;
    task->time = task_count;
    task->next = task_start;
    task_start = task;
}

bool task_dequeue(TASK_T *task)
{
	if (task == task_start)
	{
		task_start = task_start->next;
		return true;
	}

	TASK_T* prev = task_start;
	for (TASK_T* current = task_start->next; current != NULL; current = current->next)
	{
		if (current == task)
		{
			// Task found, remove from the list
			prev->next = current->next;
			return true;
		}
	}

	return false;
}

volatile int16_t task_get_count()
{
	return task_count;
}

void task_print()
{
    for (TASK_T* current = task_start; current != NULL; current = current->next)
    {
        printf_P(PSTR("count: %i, interval: %i, time: %i, priority: %i\n"), current->count, current->interval, current->time, current->priority);
    }
}
