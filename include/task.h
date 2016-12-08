#ifndef TASK_H
	#define	TASK_H
	
	#include <avr/pgmspace.h>
	#include <stdint.h>
	#include <stdbool.h>
	
	/**
	 *	Task Scheduler Header File.
	 *	@file		task.h
	 * 	@author		Thomas Hudson
	 *	@version	1.0
	 *	@date		8/9/2012
	 *
	 * 	This header file defines an interface to the "task.c" module.
	 *			
	 */
	
	#define X_ms(t)		((t)/0.5)					//!< Macro to convert "ms" of time to an appropriate number of ticks

	/**
	 * 	Typedef containing a single task.
	 *
	 * 	This typedef holds all necessary information to call an
	 *	task at a specified time.
	 */

	#ifdef __cplusplus
	extern "C" {
	#endif
	 
	typedef volatile struct TASK_S
	{
		volatile struct TASK_S *next;									//!< Pointer to the next task
        void (*func) (void* args);								//!< Pointer to a function that is executed when the task is scheduled
        void* args;                                             //!< Arguments to pass to thread function
		int16_t interval;										//!< Holds the number of ticks between task calls
		uint8_t priority;										//!< Holds the priority
		int16_t time;									//!< Holds the time for the next task call
		int8_t count;									//!< Holds the number of times to execute task
	} TASK_T;
	
	/***** Function Prototypes *****/
    void task_init(void);										//!< Initialises and starts the SysTick timer
    void task_yield(void);										//!< Scans through all tasks and calls appropriate functions
	void task_enqueue(TASK_T*, void*);                          //!< Enqueues the specified task to the task queue
	bool task_dequeue(TASK_T*);									//!< Dequeues the specified task from the task queue
    void task_print(void);
	volatile int16_t task_get_count(void);

	#define TASK_CREATE(NAME, FUNC, PRIORITY, 					\
                        INTERVAL, COUNT)                        \
	static TASK_T NAME 											\
		= 	{													\
				.func = FUNC,									\
				.time = 0,										\
				.priority = PRIORITY,							\
				.interval = INTERVAL,							\
				.count = COUNT,									\
			}

	#ifdef __cplusplus
	}
	#endif
	
#endif
