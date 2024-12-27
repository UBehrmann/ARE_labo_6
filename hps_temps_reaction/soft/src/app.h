/*
 * app.h
 *
 *  Created on: Dec 20, 2024
 *      Author: reds
 */

#ifndef SRC_APP_H_
#define SRC_APP_H_


//------------------------TYPEDEF------------------------
// Enumeration for the machine state
typedef enum {
    APP_INIT,

    APP_WAIT,
	APP_TASK,

	APP_ERROR
} App_State;
//-------------------------------------------------------



//-----------------Function prototype--------------------
void 		app_initialize();
void 		app();
void 		app_change_state(App_State new_state);
App_State	app_get_state();
//-------------------------------------------------------


#endif /* SRC_APP_H_ */
