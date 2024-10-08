/*
 * timer.h
 *
 *  Created on: May 8, 2023
 *      Author: Clinton G.
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include "stdbool.h"
#include "stm32f1xx_hal.h"

// struct timing_type {
// 	uint16_t		ticks10ms;
// 	uint16_t		ticks100ms;
// 	uint16_t		ticks500ms;

// 	bool		flag_10ms_tick;
// 	bool		flag_100ms_tick;
// 	bool		flag_500ms_tick;

// 	bool		led_fast_blink;

// };


typedef struct timing_type {
    
	uint16_t		ticks10ms;
	uint16_t		ticks100ms;
	uint16_t		ticks500ms;

	bool			flag_10ms_tick;
	bool			flag_100ms_tick;
	bool			flag_500ms_tick;

	bool			led_fast_blink;

	uint16_t		timer_100ms_cntr;
	bool			timer_100ms_running;

} timing_type;


/**
 * @brief
 * TODO need to comment
 */
void init_timer ( void );


#endif /* INC_TIMER_H_ */
