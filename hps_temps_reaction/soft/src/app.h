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
//-------------------------------------------------------



//------------------------TYPEDEF------------------------
// Enumeration for the machine state
typedef enum {
    APP_INIT,

    APP_WAIT,
	APP_TASK,

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
//-------------------------------------------------------



//-----------------Function prototype--------------------
void 		app_initialize();
void 		app();
void 		app_change_state(App_State new_state);
App_State	app_get_state();
//-------------------------------------------------------


#endif /* SRC_APP_H_ */
