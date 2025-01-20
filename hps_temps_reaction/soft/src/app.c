// Standard libraries
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>


// Application-specific headers
#include "app.h"
#include "axi_lw.h"
#include "uart/uart.h"
#include "interrupt/exceptions.h"

// Printf
int __auto_semihosting;


//-----------------Global state variable-----------------
App_State state;
const char* app_description =
		"Bienvenue dans l'application de mesure du temps de réaction !\n\n"
		"Instructions :\n"
		"1. Pour démarrer une mesure, appuyez sur le bouton KEY1.\n"
		"2. Attendez l'apparition du symbole de début sur le carré de LEDs.\n"
		"3. Dès qu'il apparaît, appuyez sur KEY0 le plus rapidement possible pour stopper la mesure.\n"
		"4. Consultez le résultat sur les afficheurs 7 segments et dans la console UART.\n\n"
		"Options supplémentaires via les interrupteurs (SW3-0) :\n"
		"- SW0=1 : Meilleur temps de réaction (ms).\n"
		"- SW1=1 : Plus mauvais temps de réaction (ms).\n"
		"- SW2=1 : Nombre d'erreurs (appui sur KEY0 avant le début).\n"
		"- SW3=1 : Nombre total de tentatives.\n"
		"- SWx=0 : Dernier temps de réaction (ms).\n\n"
		"Bonne chance et amusez-vous !\n";
//-------------------------------------------------------



//-----------------IMPLEMENTATION ISRs--------------------
void fpga_ISR(void){
    printf("Interruption FPGA KEY0 (status : %lX) ! \n", read_itp_status());
    write_itp_clear();
    printf("After clear (status : %lX) ! \n", read_itp_status());
}
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
    printf("Laboratoire: Mesure du temps de reaction \n");

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
	            // Affichage des IDs
	            printf("Constante ID : %lX\n", AXI_LW_REG(0));
	            printf("USER ID : %lX\n", read_user_id());

	            // Afficher via UART un message qui décrit comment utiliser l'app
	            write_str_uart(UART0_BASE_ADD, app_description);

	            // Lire status max 10
	            /*if(read_mx10_status() != 1){
	            	printf("Max10 status not valid : %d\n", read_mx10_status());
	            	printf("End of programm\n");
	            	//return;
	            }*/

	            // Eteindre les leds
	            write_leds(0);

	            // Affiche de la valeur 0 sur les hex
	            write_hex0(0, true);
	            write_hex1(0, true);
	            write_hex2(0, true);
	            write_hex3(0, true);

	            // Eteindre toutes les leds de la max10
				write_max10_cs(4);
				write_max10_data(1);

				write_max10_cs(5);
				write_max10_data(2);

				write_max10_cs(8);
				write_max10_data(4);

				write_max10_cs(9);
				write_max10_data(8);

				write_max10_cs(10);
				write_max10_data(16);

	            //Changement d'état
	            app_change_state(APP_WAIT);
	            break;
	        }

	        //////////////////////////////////////

	        case APP_WAIT:{
	        	printf("---WAIT---\n");

	            //Changement d'état
	        	app_change_state(APP_TASK);
	            break;
	        }

		    //////////////////////////////////////

	        case APP_TASK:{
	        	printf("---TASK---\n");

	            //Changement d'état
	        	app_change_state(APP_WAIT);
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
