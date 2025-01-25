/*
 * app.h
 *
 *  Created on: Dec 20, 2024
 *      Author: reds
 */

#ifndef SRC_APP_H_
#define SRC_APP_H_



//------------------Standard libraries-------------------
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
//-------------------------------------------------------



//------------------------DEFINE-------------------------
#define NB_KEYS 3
#define NB_SWITCHES 10
#define SYS_FREQUENCY 50000000
#define CLOCK_FREQUENCY 50000000 // 50 MHz
//-------------------------------------------------------



//------------------------MACROS-------------------------
#define TICKS_TO_MILLISECONDS(ticks) ((ticks) / (CLOCK_FREQUENCY / 1000))
//-------------------------------------------------------



//------------------------TYPEDEF------------------------
// Enumeration for the machine state
typedef enum {
    APP_INIT,

    APP_WAIT,
	APP_START_GAME,
	APP_INIT_GAME,

	APP_ERROR
} App_State;
// Structure for store the inputs
typedef struct {
	bool 		key_value[NB_KEYS];
	uint32_t 	keys_value;
	bool 		key_press[NB_KEYS];
	uint32_t 	keys_press;
} Keys_values;

typedef struct {
	bool switch_value[NB_SWITCHES];
	uint32_t switches_value;
}Switches_values;

typedef struct {
	Keys_values keys;
	Switches_values switches;
} App_Inputs;
typedef struct {
	uint32_t best_time;
	uint32_t worst_time;
	uint32_t last_time;
} Game_times;
typedef struct {
	uint32_t errors_count;
	uint32_t trys_count;
} Game_stats;
//-------------------------------------------------------



//-----------------Function prototype--------------------
void 		app_initialize();
void 		app();
void 		app_change_state(App_State new_state);
App_State	app_get_state();
//-------------------------------------------------------


#endif /* SRC_APP_H_ */
