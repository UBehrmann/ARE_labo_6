#include "avalon_functions.h"



///////////////////////////////////////////////////////////////////////////////////////////////////////////
const int int_to_hex[16] = {
        0x3F, // 0 : segments A, B, C, D, E, F allumés
        0x06, // 1 : segments B, C allumés
        0x5B, // 2 : segments A, B, G, E, D allumés
        0x4F, // 3 : segments A, B, G, C, D allumés
        0x66, // 4 : segments F, G, B, C allumés
        0x6D, // 5 : segments A, F, G, C, D allumés
        0x7D, // 6 : segments A, F, G, E, C, D allumés
        0x07, // 7 : segments A, B, C allumés
        0x7F, // 8 : tous les segments allumés
        0x6F  // 9 : segments A, B, G, F, C, D allumés
};



///////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t 	read_user_id(void) {
    return AVALON_GET_VALUE(AVALON_USER_ID_REG, AVALON_USER_ID_MASK, AVALON_USER_ID_SHIFT, AVALON_USER_ID_INVERSE_VALUE);
}

uint32_t 	read_keys(void) {
    return AVALON_GET_VALUE(AVALON_KEYS_REG, AVALON_KEYS_MASK, AVALON_KEYS_SHIFT, AVALON_KEYS_INVERSE_VALUE);
}

uint32_t 	read_switchs(void) {
    return AVALON_GET_VALUE(AVALON_SWITCHS_REG, AVALON_SWITCHS_MASK, AVALON_SWITCHS_SHIFT, AVALON_SWITCHS_INVERSE_VALUE);
}
uint32_t 	read_leds(void) {
    return AVALON_GET_VALUE(AVALON_LEDS_REG, AVALON_LEDS_MASK, AVALON_LEDS_SHIFT, AVALON_LEDS_INVERSE_VALUE);
}
uint32_t 	read_hex0(void) {
    return AVALON_GET_VALUE(AVALON_HEX_REG, AVALON_HEX0_MASK, AVALON_HEX0_SHIFT, AVALON_HEX0_INVERSE_VALUE);
}
uint32_t 	read_hex1(void) {
    return AVALON_GET_VALUE(AVALON_HEX_REG, AVALON_HEX1_MASK, AVALON_HEX1_SHIFT, AVALON_HEX1_INVERSE_VALUE);
}
uint32_t 	read_hex2(void) {
    return AVALON_GET_VALUE(AVALON_HEX_REG, AVALON_HEX2_MASK, AVALON_HEX2_SHIFT, AVALON_HEX2_INVERSE_VALUE);
}
uint32_t 	read_hex3(void) {
    return AVALON_GET_VALUE(AVALON_HEX_REG, AVALON_HEX3_MASK, AVALON_HEX3_SHIFT, AVALON_HEX3_INVERSE_VALUE);
}
//-------------------------------------------------------------
uint32_t	read_itp_status(void){
	return AVALON_GET_VALUE(AVALON_ITP_STATUS_REG, AVALON_ITP_STATUS_MASK, AVALON_ITP_STATUS_SHIFT, AVALON_ITP_STATUS_INVERSE_VALUE);
}
uint32_t	read_itp_mask(void){
	return AVALON_GET_VALUE(AVALON_ITP_MASK_REG, AVALON_ITP_MASK_MASK, AVALON_ITP_MASK_SHIFT, AVALON_ITP_MASK_INVERSE_VALUE);
}
//-------------------------------------------------------------
uint32_t	read_counter_delta(void){
    return AVALON_GET_VALUE(AVALON_COUNTER_DELTA_REG, AVALON_COUNTER_DELTA_MASK, AVALON_COUNTER_DELTA_SHIFT, AVALON_COUNTER_DELTA_INVERSE_VALUE);
}
uint32_t	read_counter_error_count(void){
    return AVALON_GET_VALUE(AVALON_COUNTER_ERROR_COUNT_REG, AVALON_COUNTER_ERROR_COUNT_MASK, AVALON_COUNTER_ERROR_COUNT_SHIFT, AVALON_COUNTER_ERROR_COUNT_INVERSE_VALUE);
}
uint32_t	read_counter_trys_count(void){
	return AVALON_GET_VALUE(AVALON_COUNTER_TRYS_COUNT_REG, AVALON_COUNTER_TRYS_COUNT_MASK, AVALON_COUNTER_TRYS_COUNT_SHIFT, AVALON_COUNTER_TRYS_COUNT_INVERSE_VALUE);
}
//-------------------------------------------------------------
uint32_t	read_counter_cycle_count(void){
    return AVALON_GET_VALUE(AVALON_COUNTER_CYCLE_COUNT_REG, AVALON_COUNTER_CYCLE_COUNT_MASK, AVALON_COUNTER_CYCLE_COUNT_SHIFT, AVALON_COUNTER_CYCLE_COUNT_INVERSE_VALUE);
}
//-------------------------------------------------------------
uint32_t 	read_counter_spt_finished(void) {
	return AVALON_GET_VALUE(AVALON_COUNTER_SPT_FINISHED_REG, AVALON_COUNTER_SPT_FINISHED_MASK, AVALON_COUNTER_SPT_FINISHED_SHIFT, AVALON_COUNTER_SPT_FINISHED_INVERSE_VALUE);
}
//-------------------------------------------------------------
uint32_t	read_max10_status(void){
    return AVALON_GET_VALUE(AVALON_MX10_STATUS_REG, AVALON_MX10_STATUS_MASK, AVALON_MX10_STATUS_SHIFT, AVALON_MX10_STATUS_INVERSE_VALUE);
}
uint32_t	read_max10_tx_busy(void){
    return AVALON_GET_VALUE(AVALON_MX10_TX_BUSY_REG, AVALON_MX10_TX_BUSY_MASK, AVALON_MX10_TX_BUSY_SHIFT, AVALON_MX10_TX_BUSY_INVERSE_VALUE);
}
uint32_t	read_max10_cs(void){
    return AVALON_GET_VALUE(AVALON_MX10_CS_REG, AVALON_MX10_CS_MASK, AVALON_MX10_CS_SHIFT, AVALON_MX10_CS_INVERSE_VALUE);
}
uint32_t	read_max10_data(void){
    return AVALON_GET_VALUE(AVALON_MX10_DATA_REG, AVALON_MX10_DATA_MASK, AVALON_MX10_DATA_SHIFT, AVALON_MX10_DATA_INVERSE_VALUE);
}
//-------------------------------------------------------------



///////////////////////////////////////////////////////////////////////////////////////////////////////////
void 		write_leds(uint32_t value) {
    AVALON_SET_VALUE(AVALON_LEDS_REG, AVALON_LEDS_MASK, AVALON_LEDS_SHIFT, value, AVALON_LEDS_INVERSE_VALUE);
}
// Écriture dans hex0 à hex3
void 		write_hex0(uint32_t value, bool isInt) {
    if (isInt && value >= 16) return;
    if (isInt && value < 15) value = int_to_hex[value]; // Conversion si nécessaire
    AVALON_SET_VALUE(AVALON_HEX_REG, AVALON_HEX0_MASK, AVALON_HEX0_SHIFT, value, AVALON_HEX0_INVERSE_VALUE);
}

void 		write_hex1(uint32_t value, bool isInt) {
    if (isInt && value >= 16) return;
    if (isInt && value < 15) value = int_to_hex[value]; // Conversion si nécessaire
    AVALON_SET_VALUE(AVALON_HEX_REG, AVALON_HEX1_MASK, AVALON_HEX1_SHIFT, value, AVALON_HEX1_INVERSE_VALUE);
}

void 		write_hex2(uint32_t value, bool isInt) {
    if (isInt && value >= 16) return;
    if (isInt && value < 15) value = int_to_hex[value]; // Conversion si nécessaire
    AVALON_SET_VALUE(AVALON_HEX_REG, AVALON_HEX2_MASK, AVALON_HEX2_SHIFT, value, AVALON_HEX2_INVERSE_VALUE);
}

void 		write_hex3(uint32_t value, bool isInt) {
    if (isInt && value >= 16) return;
    if (isInt && value < 15) value = int_to_hex[value]; // Conversion si nécessaire
    AVALON_SET_VALUE(AVALON_HEX_REG, AVALON_HEX3_MASK, AVALON_HEX3_SHIFT, value, AVALON_HEX3_INVERSE_VALUE);
}
//-------------------------------------------------------------
void		write_itp_clear(){
	AVALON_SET_VALUE(AVALON_ITP_CLEAR_REG, AVALON_ITP_CLEAR_MASK, AVALON_ITP_CLEAR_SHIFT, 1, AVALON_ITP_CLEAR_INVERSE_VALUE);
}
void		write_itp_mask(uint32_t value){
	AVALON_SET_VALUE(AVALON_ITP_MASK_REG, AVALON_ITP_MASK_MASK, AVALON_ITP_MASK_SHIFT, value, AVALON_ITP_MASK_INVERSE_VALUE);
}
//-------------------------------------------------------------
void        write_counteur_start(uint32_t value){
    AVALON_SET_VALUE(AVALON_COUNTER_START_REG, AVALON_COUNTER_START_MASK, AVALON_COUNTER_START_SHIFT, value, AVALON_COUNTER_START_INVERSE_VALUE);
}
void        write_counteur_stop(uint32_t value){
    AVALON_SET_VALUE(AVALON_COUNTER_STOP_REG, AVALON_COUNTER_STOP_MASK, AVALON_COUNTER_STOP_SHIFT, value, AVALON_COUNTER_STOP_INVERSE_VALUE);
}
//-------------------------------------------------------------
void		write_counteur_spt_start(uint32_t value){
    AVALON_SET_VALUE(AVALON_COUNTER_SPT_START_REG, AVALON_COUNTER_SPT_START_MASK, AVALON_COUNTER_SPT_START_SHIFT, value, AVALON_COUNTER_SPT_START_INVERSE_VALUE);
}
void		write_counteur_spt_stop(uint32_t value){
    AVALON_SET_VALUE(AVALON_COUNTER_SPT_STOP_REG, AVALON_COUNTER_SPT_STOP_MASK, AVALON_COUNTER_SPT_STOP_SHIFT, value, AVALON_COUNTER_SPT_STOP_INVERSE_VALUE);
}
void		write_counteur_spt_setpoint(uint32_t value){
    AVALON_SET_VALUE(AVALON_COUNTER_SPT_SETPOINT_REG, AVALON_COUNTER_SPT_SETPOINT_MASK, AVALON_COUNTER_SPT_SETPOINT_SHIFT, value, AVALON_COUNTER_SPT_SETPOINT_INVERSE_VALUE);
}
//-------------------------------------------------------------
void        write_max10_cs(uint32_t value){
    AVALON_SET_VALUE(AVALON_MX10_CS_REG, AVALON_MX10_CS_MASK, AVALON_MX10_CS_SHIFT, value, AVALON_MX10_CS_INVERSE_VALUE);
}
void        write_max10_data(uint32_t value){
    AVALON_SET_VALUE(AVALON_MX10_DATA_REG, AVALON_MX10_DATA_MASK, AVALON_MX10_DATA_SHIFT, value, AVALON_MX10_DATA_INVERSE_VALUE);
    while (AVALON_GET_VALUE(AVALON_MX10_TX_BUSY_REG, AVALON_MX10_TX_BUSY_MASK, AVALON_MX10_TX_BUSY_SHIFT, AVALON_MX10_TX_BUSY_INVERSE_VALUE)) {}
}
//-------------------------------------------------------------

