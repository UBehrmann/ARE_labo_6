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
uint32_t	read_mx10_status(void){
    return AVALON_GET_VALUE(AVALON_MX10_REG, AVALON_MX10_STATUS_MASK, AVALON_MX10_STATUS_SHIFT, AVALON_MX10_STATUS_INVERSE_VALUE);
}
uint32_t	read_mx10_ready(void){
    return AVALON_GET_VALUE(AVALON_MX10_REG, AVALON_MX10_READY_MASK, AVALON_MX10_READY_SHIFT, AVALON_MX10_READY_INVERSE_VALUE);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
void 		write_leds(uint32_t value) {
    AVALON_SET_VALUE(AVALON_LEDS_REG, AVALON_LEDS_MASK, AVALON_LEDS_SHIFT, value);
}
// Écriture dans hex0 à hex3
void 		write_hex0(uint32_t value, bool isInt) {
    if (isInt && value >= 16) return;
    if (isInt && value < 15) value = int_to_hex[value]; // Conversion si nécessaire
    AVALON_SET_VALUE(AVALON_HEX_REG, AVALON_HEX0_MASK, AVALON_HEX0_SHIFT, value);
}

void 		write_hex1(uint32_t value, bool isInt) {
    if (isInt && value >= 16) return;
    if (isInt && value < 15) value = int_to_hex[value]; // Conversion si nécessaire
    AVALON_SET_VALUE(AVALON_HEX_REG, AVALON_HEX1_MASK, AVALON_HEX1_SHIFT, value);
}

void 		write_hex2(uint32_t value, bool isInt) {
    if (isInt && value >= 16) return;
    if (isInt && value < 15) value = int_to_hex[value]; // Conversion si nécessaire
    AVALON_SET_VALUE(AVALON_HEX_REG, AVALON_HEX2_MASK, AVALON_HEX2_SHIFT, value);
}

void 		write_hex3(uint32_t value, bool isInt) {
    if (isInt && value >= 16) return;
    if (isInt && value < 15) value = int_to_hex[value]; // Conversion si nécessaire
    AVALON_SET_VALUE(AVALON_HEX_REG, AVALON_HEX3_MASK, AVALON_HEX3_SHIFT, value);
}
