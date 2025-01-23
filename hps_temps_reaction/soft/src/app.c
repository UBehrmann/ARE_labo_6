
// Application-specific headers
#include "app.h"
#include "axi_lw.h"
#include "uart/uart.h"
#include "interrupt/exceptions.h"

// Printf
int __auto_semihosting;



//---------------------Global defines--------------------
#define MAX10_CS_LEDS_SEC_15_01					0
#define MAX10_CS_LEDS_SEC_30_16					1
#define MAX10_CS_LEDS_SEC_45_31					2
#define MAX10_CS_LEDS_SEC_60_46					3
#define MAX10_CS_LEDS_LINE_15_00				4
#define MAX10_CS_LEDS_LINE_31_16				5
#define MAX10_CS_LEDS_SQUARE_35t31_25t21_15t11	6
#define MAX10_CS_LEDS_SQUARE_55t51_45t41		7
#define MAX10_CS_LEDS_HOUR_04_01				8
#define MAX10_CS_LEDS_HOUR_08_05				9
#define MAX10_CS_LEDS_HOUR_12_09				10
//-------------------------------------------------------



//-----------------Global state variable-----------------
App_State state;
App_Inputs app_inputs;
const char* app_description =
		"Bienvenue dans l'application de mesure du temps de réaction !\n\n"
		"Instructions :\n"
		"1.Pour démarrer une mesure, appuyez sur le bouton KEY1.\n"
		"2.Attendez l'apparition du symbole de début sur le carré de LEDs.\n"
		"3.Dès qu'il apparaît, appuyez sur KEY0 le plus rapidement possible pour stopper la mesure.\n"
		"4.Consultez le résultat sur les afficheurs 7 segments et dans la console UART.\n\n"
		"Options supplémentaires via les interrupteurs (SW3-0) :\n"
		"- SW0=1 : Meilleur temps de réaction (ms).\n"
		"- SW1=1 : Plus mauvais temps de réaction (ms).\n"
		"- SW2=1 : Nombre d'erreurs (appui sur KEY0 avant le début).\n"
		"- SW3=1 : Nombre total de tentatives.\n"
		"- SWx=0 : Dernier temps de réaction (ms).\n\n"
		"Bonne chance et amusez-vous !\n";
const char* max10_status_msg[4] = {
		"Configuration non valide",
		"Configuration valide",
		"Configuration reserve",
		"Configuration reserve",
};
uint32_t max10_cs_values[] = {
		MAX10_CS_LEDS_SEC_15_01,
		MAX10_CS_LEDS_SEC_30_16,
		MAX10_CS_LEDS_SEC_45_31,
		MAX10_CS_LEDS_SEC_60_46,
		MAX10_CS_LEDS_LINE_15_00,
		MAX10_CS_LEDS_LINE_31_16,
		MAX10_CS_LEDS_SQUARE_35t31_25t21_15t11,
		MAX10_CS_LEDS_SQUARE_55t51_45t41,
		MAX10_CS_LEDS_HOUR_04_01,
		MAX10_CS_LEDS_HOUR_08_05,
		MAX10_CS_LEDS_HOUR_12_09
};
//-------------------------------------------------------



//-----------------IMPLEMENTATION ISRs--------------------
void fpga_ISR(void){
    printf("Interruption FPGA KEY0 (status : %lX) ! \n", read_itp_status());
    write_itp_clear();
    printf("After clear (status : %lX) ! \n", read_itp_status());
}
//--------------------------------------------------------



//--------------Prototype local functions-----------------
void init_inputs_value();
void read_inputs_value();
//--------------------------------------------------------



//---------------Prototype APP functions------------------
void app_change_state(App_State new_state);
App_State app_get_state();
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

	// Initialisation champ par champ
    init_inputs_value();

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
	uint32_t value_to_wait = 0;
	bool flag = false;
	while(1){
//------------------------State Machine------------------------
	    switch (app_get_state()) {
	        case APP_INIT:{
	            // Affichage des IDs
	            printf("Constante ID : %lX\n", AXI_LW_REG(0));
	            printf("USER ID : %lX\n", read_user_id());
	            uint32_t max10_status = read_max10_status();
	            printf("Max10 status : %d (%s)\n",max10_status, max10_status_msg[max10_status]);
	            if(max10_status != 1){
	            	printf("End of programm\n");
	            }

	            // Afficher via UART un message qui décrit comment utiliser l'app
	            write_str_uart(UART0_BASE_ADD, app_description);

	            // Eteindre les leds
	            write_leds(0);

	            // Affiche de la valeur 0 sur les hex
	            write_hex0(0, true);
	            write_hex1(0, true);
	            write_hex2(0, true);
	            write_hex3(0, true);

	            // Eteindre toutes les leds de la max10
	            uint8_t nb_max10_cs = (sizeof(max10_cs_values)/sizeof(max10_cs_values[0]));
				for(uint8_t i = 0; i < nb_max10_cs; i++){
					write_max10_cs(max10_cs_values[i]);
					write_max10_data(0);
				}
	            //Changement d'état
	            app_change_state(APP_WAIT);
	            break;
	        }

	        //////////////////////////////////////

	        case APP_WAIT:{
	        	printf("---WAIT---\n");
	        	//Lecture des entrées
	        	read_inputs_value();


	        	if(flag && (read_counter_delta() >= value_to_wait)){
		        	//Affichage du state : DEBUT;
		        	write_max10_cs(MAX10_CS_LEDS_SQUARE_35t31_25t21_15t11);
		        	write_max10_data(0b0100011000111111);

		        	write_max10_cs(MAX10_CS_LEDS_SQUARE_55t51_45t41);
		        	write_max10_data(0b0000001111110001);
		        	write_counteur_stop(1);
		        	flag = false;
	        	}

	            //Changement d'état
	        	app_change_state(app_inputs.keys.key_press[1] == true ? APP_TASK : APP_WAIT);
	            break;
	        }

		    //////////////////////////////////////

	        case APP_TASK:{
	        	printf("---TASK---\n");
	        	value_to_wait = read_counter_cycle_count(); //0..3s
	        	value_to_wait += 50000000; 					//+1s -> 1..4s

	        	// Partie centrale du symbole (croix verte)
	        	write_max10_cs(MAX10_CS_LEDS_SQUARE_35t31_25t21_15t11);
	        	write_max10_data(0b0111110010000100);

	        	write_max10_cs(MAX10_CS_LEDS_SQUARE_55t51_45t41);
	        	write_max10_data(0b0000000010000100);

	        	write_counteur_start(1);
	        	flag = true;

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



//-------------------local functions----------------------
void init_inputs_value(){
	for (int i = 0; i < 4; i++) {
	    app_inputs.keys.key_value[i] = 0;
	    app_inputs.keys.key_press[i] = 0;
	}
	app_inputs.keys.keys_value = 0;
	app_inputs.keys.keys_press = 0;

	for (int i = 0; i < 10; i++) {
	    app_inputs.switches.switch_value[i] = 0;
	}
	app_inputs.switches.switches_value = 0;
}
void read_k() {
	uint32_t val_keys = read_keys();
	for(uint8_t i = 0; i < NB_KEYS; ++i){
		bool key_value = (val_keys >> i) & 0x1;
		app_inputs.keys.key_press[i] = key_value > app_inputs.keys.key_value[i] ? true : false;
		app_inputs.keys.key_value[i] = key_value;
		app_inputs.keys.keys_press += (app_inputs.keys.key_press[i] << i);
	}
	app_inputs.keys.keys_value = val_keys;
}
void read_s() {
	uint32_t val_switchs = read_switchs();
	for(uint8_t i = 0; i < NB_SWITCHES; ++i) {
		bool switch_value = (val_switchs >> i) & 0x1;
		app_inputs.switches.switch_value[i] = switch_value;
	}
	app_inputs.switches.switches_value = val_switchs;
}
void read_inputs_value(){
	read_k();
	read_s();
}
//--------------------------------------------------------
