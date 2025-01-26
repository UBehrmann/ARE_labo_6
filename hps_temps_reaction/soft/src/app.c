
// Application-specific headers
#include "app.h"
#include "axi_lw.h"
#include "uart/uart.h"
#include "interrupt/exceptions.h"
#include "time.h"

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
#define MAX10_CS_LEDS_HOUR						11

#define MAX10_LEDS_SQUARE_NB_BITS_Y				5
#define MAX10_LEDS_SQUARE_NB_BITS_X				5

#define ON										true
#define OFF 									false

#define PRINTF_ENABLE							true
//-------------------------------------------------------



//-----------------Global state variable-----------------
App_State state;
App_Inputs app_inputs;
Game_times game_times;
Game_stats game_stats;
const char* app_description =
		"Bienvenue dans l'application de mesure du temps de réaction !\r\n"
		"Instructions :\r\n"
		"1.Démarer mesure via KEY1.\r\n"
		"2.Attendre symbole début\r\n"
		"3.Dès symbole départ, appuyez sur KEY0.\r\n"
		"4.Consultez le résultat sur les afficheurs 7 segments et dans la console UART.\r\n"
		"Options supplémentaires via les interrupteurs (SW3-0) :\r\n"
		"- SW0=1 : Meilleur temps de réaction (ms).\r\n"
		"- SW1=1 : Plus mauvais temps de réaction (ms).\r\n"
		"- SW2=1 : Nombre d'erreurs (appui sur KEY0 avant le début).\r\n"
		"- SW3=1 : Nombre total de tentatives.\r\n"
		"- SWx=0 : Dernier temps de réaction (ms).\r\n"
		"Bonne chance et amusez-vous !\r\n";
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
		MAX10_CS_LEDS_HOUR,
		MAX10_CS_LEDS_SQUARE_35t31_25t21_15t11,
		MAX10_CS_LEDS_SQUARE_55t51_45t41
};
bool max10_leds_square_wait_value[MAX10_LEDS_SQUARE_NB_BITS_Y][MAX10_LEDS_SQUARE_NB_BITS_X] ={
		{	OFF,	OFF,	ON,		OFF,	OFF},
		{	OFF,	OFF,	ON,		OFF,	OFF},
		{	ON,		ON,		ON,		ON,		ON},
		{	OFF,	OFF,	ON,		OFF,	OFF},
		{	OFF,	OFF,	ON,		OFF,	OFF},
};
bool max10_leds_square_start_value[MAX10_LEDS_SQUARE_NB_BITS_Y][MAX10_LEDS_SQUARE_NB_BITS_X] ={
		{	ON,		ON,		ON,		ON,		ON},
		{	ON,		OFF,	OFF,	OFF,	ON},
		{	ON,		OFF,	OFF,	OFF,	ON},
		{	ON,		OFF,	OFF,	OFF,	ON},
		{	ON,		ON,		ON,		ON,		ON},
};
bool max10_leds_square_stop_value[MAX10_LEDS_SQUARE_NB_BITS_Y][MAX10_LEDS_SQUARE_NB_BITS_X] ={
		{	OFF,	OFF,	OFF,	OFF,	OFF},
		{	OFF,	ON,		ON,		ON,		OFF},
		{	OFF,	ON,		ON,		ON,		OFF},
		{	OFF,	ON,		ON,		ON,		OFF},
		{	OFF,	OFF,	OFF,	OFF,	OFF},
};
//-------------------------------------------------------



//--------------Prototype local functions-----------------
void init_inputs_value();
void read_inputs_value();
//-----------------------
void init_game_times();
void update_game_time(uint32_t time);
void init_game_stats();
void update_game_stats(uint32_t errors_count, uint32_t trys_count);
void display_game_data();
//-----------------------
uint32_t 	convert_max10_leds_square_value(uint32_t cs_square, bool* square_value);
uint32_t 	convert_counteur_delta_to_time(uint32_t counteur_delta);
uint32_t 	convert_time_to_counteur_delta(uint32_t time);
void		wait_ms(uint32_t ms);
void 		turn_off_hour_leds();
void 		split_digits(uint32_t value, uint8_t* digits_tb, size_t size);
//--------------------------------------------------------



//---------------Prototype APP functions------------------
void app_change_state(App_State new_state);
App_State app_get_state();
//--------------------------------------------------------



//-----------------IMPLEMENTATION ISRs--------------------
void fpga_ISR(void){
    printf("Interruption FPGA KEY0 (status : %lX) ! \n", read_itp_status());

    // Récupérer le temps de réaction
    write_counteur_stop(1);
    uint32_t reaction_time = convert_counteur_delta_to_time(read_counter_delta());
    uint32_t errors_count = read_counter_error_count();
    uint32_t trys_count = read_counter_trys_count();
    update_game_time(reaction_time);
    update_game_stats(errors_count, trys_count);

	// Affichage symbole : FIN
	write_max10_cs(MAX10_CS_LEDS_SQUARE_35t31_25t21_15t11);
	write_max10_data(convert_max10_leds_square_value(
			MAX10_CS_LEDS_SQUARE_35t31_25t21_15t11,
			max10_leds_square_stop_value));

	write_max10_cs(MAX10_CS_LEDS_SQUARE_55t51_45t41);
	write_max10_data(convert_max10_leds_square_value(
			MAX10_CS_LEDS_SQUARE_55t51_45t41,
			max10_leds_square_stop_value));

	//Afficher les données en UART
    display_game_data();

	//Clear l'interruption
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

    //Désactivé l'interruption de l'interface
    write_itp_mask(1);

    //Initialisation des interuption
    app_init_interrupt();

	// Initialisation champ par champ
    init_inputs_value();

    // Intialisation des temps enregistré du jeux
    init_game_times();

    // Initialisation des stats du jeux
    init_game_stats();

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
		            //Changement d'état
		            app_change_state(APP_ERROR);
		            break;
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
					if(max10_cs_values[i] == MAX10_CS_LEDS_HOUR){
						turn_off_hour_leds();
						continue;
					}
					write_max10_cs(max10_cs_values[i]);
					write_max10_data(0);

					wait_ms(1);
				}

			    //Activé l'interruption de l'interface
			    write_itp_mask(0);

	            //Changement d'état
	            app_change_state(APP_WAIT);
	            break;
	        }

	        //////////////////////////////////////

	        case APP_WAIT:{
	        	printf("---WAIT---\n");
	        	//Lecture des entrées
	        	read_inputs_value();

	        	//
	        	uint32_t value_to_display_hex = 0;
	        	if(app_inputs.switches.switch_value[0]) value_to_display_hex = game_times.best_time;
	        	if(app_inputs.switches.switch_value[1]) value_to_display_hex = game_times.worst_time;
	        	if(app_inputs.switches.switch_value[2]) value_to_display_hex = game_stats.errors_count;
	        	if(app_inputs.switches.switch_value[3]) value_to_display_hex = game_stats.trys_count;
	        	if((app_inputs.switches.switches_value & 0xF) == 0) value_to_display_hex = game_times.last_time;
	        	uint8_t nb_digits = 4;
	        	uint8_t digits[nb_digits];
	        	split_digits(value_to_display_hex, digits, nb_digits);
	            write_hex0(digits[3], true);
	            write_hex1(digits[2], true);
	            write_hex2(digits[1], true);
	            write_hex3(digits[0], true);


	        	//Changement d'état
	        	if(read_counter_spt_finished()){
	        		app_change_state(APP_START_GAME);
	        	} else {
	        		app_change_state(app_inputs.keys.key_press[1] == true ? APP_INIT_GAME : APP_WAIT);
	        	}
	            break;
	        }

	        //////////////////////////////////////

	        case APP_START_GAME:{
	        	printf("---START_GAME---\n");
        		write_counteur_spt_stop(1);

	        	// Affichage symbole : START
	        	write_max10_cs(MAX10_CS_LEDS_SQUARE_35t31_25t21_15t11);
	        	write_max10_data(convert_max10_leds_square_value(
	        			MAX10_CS_LEDS_SQUARE_35t31_25t21_15t11,
						max10_leds_square_start_value));

	        	write_max10_cs(MAX10_CS_LEDS_SQUARE_55t51_45t41);
	        	write_max10_data(convert_max10_leds_square_value(
	        			MAX10_CS_LEDS_SQUARE_55t51_45t41,
						max10_leds_square_start_value));

	        	write_counteur_start(1);

	            //Changement d'état
	        	app_change_state(APP_WAIT);
	        	break;
	        }

		    //////////////////////////////////////

	        case APP_INIT_GAME:{
	        	printf("---INIT_GAMEl---\n");
	        	value_to_wait = read_counter_cycle_count(); //0..3s
	        	value_to_wait += 50000000; 					//+1s -> 1..4s

	        	// Affichage symbole : ATTENTE
	        	write_max10_cs(MAX10_CS_LEDS_SQUARE_35t31_25t21_15t11);
	        	write_max10_data(convert_max10_leds_square_value(
	        			MAX10_CS_LEDS_SQUARE_35t31_25t21_15t11,
						max10_leds_square_wait_value));

	        	write_max10_cs(MAX10_CS_LEDS_SQUARE_55t51_45t41);
	        	write_max10_data(convert_max10_leds_square_value(
	        			MAX10_CS_LEDS_SQUARE_55t51_45t41,
						max10_leds_square_wait_value));

	        	// Démarrage du compteur pour starter le jeu de réaction
	        	write_counteur_spt_setpoint(value_to_wait);
	        	write_counteur_spt_start(1);

	            //Changement d'état
	        	app_change_state(APP_WAIT);
	        	break;
	        }

		    //////////////////////////////////////

	        case APP_ERROR:{
	        	printf("---ERROR---\n");
	        	return;
	        }
	    }
	    //-------------------------------------------------------------
	}
}
//--------------------------------------------------------



//-------------------local functions----------------------
void init_game_times(){
	game_times.best_time = UINT32_MAX;
	game_times.worst_time = 0;
	game_times.last_time = 0;
}
void update_game_time(uint32_t time){
	if(time < game_times.best_time) game_times.best_time = time;
	if(time > game_times.worst_time) game_times.worst_time = time;
	game_times.last_time = time;
}
void init_game_stats(){
	game_stats.errors_count = 0;
	game_stats.trys_count = 0;
}
void update_game_stats(uint32_t errors_count, uint32_t trys_count){
	game_stats.errors_count = errors_count;
	game_stats.trys_count = trys_count;
}
void display_game_data() {
    char buffer[100];

    write_str_uart(UART0_BASE_ADD, "---------------------------------------------\r\n");

    snprintf(buffer, sizeof(buffer), "Temps réaction [ms] : %d \r\n", game_times.last_time);
    write_str_uart(UART0_BASE_ADD, buffer);

    snprintf(buffer, sizeof(buffer), "Meilleur temps réaction [ms] : %d \r\n", game_times.best_time);
    write_str_uart(UART0_BASE_ADD, buffer);

    snprintf(buffer, sizeof(buffer), "Pire temps réaction [ms] : %d \r\n", game_times.worst_time);
    write_str_uart(UART0_BASE_ADD, buffer);

    snprintf(buffer, sizeof(buffer), "Nombre d'essais : %d \r\n", game_stats.trys_count);
    write_str_uart(UART0_BASE_ADD, buffer);

    snprintf(buffer, sizeof(buffer), "Nombre d'erreurs : %d \r\n", game_stats.errors_count);
    write_str_uart(UART0_BASE_ADD, buffer);

    write_str_uart(UART0_BASE_ADD, "---------------------------------------------\r\n");
}
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
uint32_t	convert_counteur_delta_to_time(uint32_t counteur_delta){
	return TICKS_TO_MILLISECONDS(counteur_delta);
}
uint32_t 	convert_time_to_counteur_delta(uint32_t time) {
    return MILLISECONDS_TO_TICKS(time);
}
void		wait_ms(uint32_t ms){
	uint32_t setpoint = convert_time_to_counteur_delta(ms);

	// Démarrage du compteur
	write_counteur_spt_setpoint(setpoint);
	write_counteur_spt_start(1);

	// Attendre
	while(!read_counter_spt_finished()) {}

	// Arreter le compteur
	write_counteur_spt_stop(1);

}
uint32_t 	convert_max10_leds_square_value(uint32_t cs_square, bool* square_value){
	uint8_t start_line	= (cs_square == MAX10_CS_LEDS_SQUARE_35t31_25t21_15t11) ? 0 : 3;
	uint8_t stop_line 	= (cs_square == MAX10_CS_LEDS_SQUARE_35t31_25t21_15t11) ? 2 : 4;

	uint32_t value 		= 0;
    uint8_t bit_index 	= 0;

	for(uint8_t i = start_line; i <= stop_line; i++){
		for(uint8_t j = 0; j < MAX10_LEDS_SQUARE_NB_BITS_X; j++){
			if (*(square_value + i * MAX10_LEDS_SQUARE_NB_BITS_X + j)) value |= (1 << bit_index);
			bit_index++;
		}
	}
	return value;
}
void 		split_digits(uint32_t value, uint8_t* digits_tb, size_t size){
    for (int i = size - 1; i >= 0; i--) {
    	digits_tb[i] = value % 10;  // Extraire le dernier chiffre
        value /= 10;            // Réduire la valeur
    }
}
void 		turn_off_hour_leds(){
    uint32_t hour_colors[] = {0x0000, 0x4444, 0x8888, 0xCCCC};
    uint32_t hour_cs_reg[] = {MAX10_CS_LEDS_HOUR_04_01, MAX10_CS_LEDS_HOUR_08_05, MAX10_CS_LEDS_HOUR_12_09};

    for(uint8_t i = 0; i < 3; i++){
        for (uint8_t j = 0; j < 4; j++){
            write_max10_cs(hour_cs_reg[i]);
            write_max10_data(hour_colors[j]);
        }
    }
}
//--------------------------------------------------------
