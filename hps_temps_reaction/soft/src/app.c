// Standard libraries


// Application-specific headers
#include "axi_lw.h"
#include "uart/uart.h"
#include "interrupt/exceptions.h"

// Printf
int __auto_semihosting;


//-----------------Global state variable-----------------
App_State state;
//-------------------------------------------------------



//--------------Prototype local functions-----------------
//--------------------------------------------------------



//--------------------APP functions-----------------------
void app_init_uart(){
    // Configurer le baudrate à 9600
    configure_baud_rate(UART0_BASE_ADD, 100000000, 9600);
    // Configurer la ligne série (8 bits de données, pas de parité, 1 bit de stop)
    configure_line(UART0_BASE_ADD);
    // Activer les FIFO
    enable_fifo(UART0_BASE_ADD);
}
void app_init_interrupt(){
    // Initialiser la pile pour le mode IRQ
    set_A9_IRQ_stack();
    // Configurer le GIC pour gérer les interruptions du timer
    config_GIC();
    // Activer les interruptions globales sur le processeur ARM
    enable_A9_interrupts();
}

// Initializes registers as required
void app_initialize(){
    printf("Laboratoire: Mesurle du temps de reaction \n");

    //Initialisation de l'UART
    app_init_uart();

    //Initialisation des interuption
    app_init_interrupt();

    //Changement d'état
    app_change_state(APP_INIT);
}

// Application state change
void app_change_state(App_State new_state){
	state = new_state;
}

// Get application state
App_State app_get_state(){
	return state;
}
//--------------------------------------------------------


//--------------------APLICATION--------------------
void app(){
	while(1){
//------------------------State Machine------------------------
	    switch (app_get_state()) {
	        case APP_INIT:{
	            // Affichage de la l'ID constant
	        	printf("constanteID: 0x%X\n", (unsigned int)AXI_LW_REG(0));



	            //Changement d'état
	            app_change_state(APP_WAIT);
	            break;
	        }

	        //////////////////////////////////////

	        case APP_WAIT:{
	        	printf("---WAIT---\n");

	        	//Lecture des entrées
	        	read_inputs_value();

	            //Changement d'état
	        	app_change_state(next_state());
	            break;
	        }

		    //////////////////////////////////////

	        case APP_TASK:{
	        	printf("---TASK---\n");

	        	//Active le timer
	        	timer_osc1_0_start();

	            //Changement d'état
	        	app_change_state(APP_READ_INPUTS);
	        	break;
	        }

		    //////////////////////////////////////

	        case APP_ERROR:{
	        	printf("---ERROR---\n");

	            //Changement d'état
	        	app_change_state(APP_WAIT);
	        	break;
	        }
	    }
	    //-------------------------------------------------------------
	}
}
//--------------------------------------------------------
