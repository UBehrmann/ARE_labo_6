/*
 * io.h
 *
 *  Created on: Dec 20, 2024
 *      Author: reds
 */

#ifndef SRC_AVALON_AVALON_FUNCTIONS_H_
#define SRC_AVALON_AVALON_FUNCTIONS_H_



// Include
#include <stdint.h>
#include <stdbool.h>
#include "avalon.h"



//------------------------DEFINES------------------------
// Offsets des registres
#define AVALON_USER_ID_OFFSET     			0x00
#define AVALON_KEYS_OFFSET     				0x04
#define AVALON_SWITCHS_OFFSET     			0x08
#define AVALON_LEDS_OFFSET        			0x0C
#define AVALON_HEX_OFFSET       			0x10
//-------------------
#define AVALON_ITP_STATUS_OFFSET			0x14
#define AVALON_ITP_CLEAR_OFFSET				0x14
#define AVALON_ITP_MASK_OFFSET				0x18
//-------------------
#define AVALON_COUNTER_START_OFFSET			0x2C
#define AVALON_COUNTER_STOP_OFFSET			0x30
#define AVALON_COUNTER_DELTA_OFFSET			0x34
#define AVALON_COUNTER_ERROR_COUNT_OFFSET	0x38
#define AVALON_COUNTER_TRYS_COUNT_OFFSET	0x3C
//-------------------
#define AVALON_COUNTER_CYCLE_COUNT_OFFSET	0x40
//-------------------
#define AVALON_COUNTER_SPT_START_OFFSET		0x44
#define AVALON_COUNTER_SPT_STOP_OFFSET		0x48
#define AVALON_COUNTER_SPT_SETPOINT_OFFSET	0x4C
#define AVALON_COUNTER_SPT_FINISHED_OFFSET	0x50
//-------------------
#define AVALON_MX10_STATUS_OFFSET			0x1C
#define AVALON_MX10_TX_BUSY_OFFSET			0x20
#define AVALON_MX10_CS_OFFSET				0x24
#define AVALON_MX10_DATA_OFFSET				0x28
//-------------------


// Masques des registres
#define AVALON_USER_ID_MASK       			0xFFFFFFFF
#define AVALON_KEYS_MASK       				0x0000000F
#define AVALON_SWITCHS_MASK       			0x000003FF
#define AVALON_LEDS_MASK          			0x000003FF
#define AVALON_HEX0_MASK        			0x0000007F // Bits 6..0
#define AVALON_HEX1_MASK       				0x00003F80 // Bits 13..7
#define AVALON_HEX2_MASK        			0x001FC000 // Bits 20..14
#define AVALON_HEX3_MASK        			0x0FE00000 // Bits 27..21
//-------------------
#define AVALON_ITP_STATUS_MASK				0x00000003
#define AVALON_ITP_CLEAR_MASK				0x00000001
#define AVALON_ITP_MASK_MASK				0x00000001
//-------------------
#define AVALON_COUNTER_START_MASK			0x00000001
#define AVALON_COUNTER_STOP_MASK			0x00000001
#define AVALON_COUNTER_DELTA_MASK			0xFFFFFFFF
#define AVALON_COUNTER_ERROR_COUNT_MASK		0xFFFFFFFF
#define AVALON_COUNTER_TRYS_COUNT_MASK		0xFFFFFFFF
//-------------------
#define AVALON_COUNTER_CYCLE_COUNT_MASK		0xFFFFFFFF
//-------------------
#define AVALON_COUNTER_SPT_START_MASK		0x00000001
#define AVALON_COUNTER_SPT_STOP_MASK		0x00000001
#define AVALON_COUNTER_SPT_SETPOINT_MASK	0xFFFFFFFF
#define AVALON_COUNTER_SPT_FINISHED_MASK	0x00000001
//-------------------
#define AVALON_MX10_STATUS_MASK				0x00000003
#define AVALON_MX10_TX_BUSY_MASK			0x00000001
#define AVALON_MX10_CS_MASK                 0x0000000F
#define AVALON_MX10_DATA_MASK               0x0000FFFF
//-------------------


// Décalage des valeurs des registres
#define AVALON_USER_ID_SHIFT				0
#define AVALON_KEYS_SHIFT       			0
#define AVALON_SWITCHS_SHIFT       			0
#define AVALON_LEDS_SHIFT          			0
#define AVALON_HEX0_SHIFT       			0
#define AVALON_HEX1_SHIFT       			7
#define AVALON_HEX2_SHIFT       			14
#define AVALON_HEX3_SHIFT       			21
//-------------------
#define AVALON_ITP_STATUS_SHIFT				0
#define AVALON_ITP_CLEAR_SHIFT				0
#define AVALON_ITP_MASK_SHIFT				0
//-------------------
#define AVALON_COUNTER_START_SHIFT			0
#define AVALON_COUNTER_STOP_SHIFT			0
#define AVALON_COUNTER_DELTA_SHIFT			0
#define AVALON_COUNTER_ERROR_COUNT_SHIFT	0
#define AVALON_COUNTER_TRYS_COUNT_SHIFT		0
//-------------------
#define AVALON_COUNTER_CYCLE_COUNT_SHIFT	0
//-------------------
#define AVALON_COUNTER_SPT_START_SHIFT		0
#define AVALON_COUNTER_SPT_STOP_SHIFT		0
#define AVALON_COUNTER_SPT_SETPOINT_SHIFT	0
#define AVALON_COUNTER_SPT_FINISHED_SHIFT	0
//-------------------
#define AVALON_MX10_STATUS_SHIFT			0
#define AVALON_MX10_TX_BUSY_SHIFT			0
#define AVALON_MX10_CS_SHIFT                0
#define AVALON_MX10_DATA_SHIFT              0
//-------------------


// Valeur normal (par défaut des registres)
#define AVALON_USER_ID_INVERSE_VALUE		0
#define AVALON_KEYS_INVERSE_VALUE   		1
#define AVALON_SWITCHS_INVERSE_VALUE    	0
#define AVALON_LEDS_INVERSE_VALUE       	0
#define AVALON_HEX0_INVERSE_VALUE 			1
#define AVALON_HEX1_INVERSE_VALUE 			1
#define AVALON_HEX2_INVERSE_VALUE 			1
#define AVALON_HEX3_INVERSE_VALUE 			1
//-------------------
#define AVALON_ITP_STATUS_INVERSE_VALUE		0
#define AVALON_ITP_CLEAR_INVERSE_VALUE		0
#define AVALON_ITP_MASK_INVERSE_VALUE		0
//-------------------
#define AVALON_COUNTER_START_INVERSE_VALUE	0
#define AVALON_COUNTER_STOP_INVERSE_VALUE	0
#define AVALON_COUNTER_DELTA_INVERSE_VALUE	0
#define AVALON_COUNTER_ERROR_COUNT_INVERSE_VALUE	0
#define AVALON_COUNTER_TRYS_COUNT_INVERSE_VALUE		0
//-------------------
#define AVALON_COUNTER_CYCLE_COUNT_INVERSE_VALUE	0
//-------------------
#define AVALON_COUNTER_SPT_START_INVERSE_VALUE		0
#define AVALON_COUNTER_SPT_STOP_INVERSE_VALUE		0
#define AVALON_COUNTER_SPT_SETPOINT_INVERSE_VALUE	0
#define AVALON_COUNTER_SPT_FINISHED_INVERSE_VALUE	0
//-------------------
#define AVALON_MX10_STATUS_INVERSE_VALUE	0
#define AVALON_MX10_TX_BUSY_INVERSE_VALUE	0
#define AVALON_MX10_CS_INVERSE_VALUE        0
#define AVALON_MX10_DATA_INVERSE_VALUE      0
//-------------------
//-------------------------------------------------------



//-----------------------REGISTERS-----------------------
#define AVALON_USER_ID_REG      	(AVALON_REG(AVALON_USER_ID_OFFSET))
#define AVALON_KEYS_REG      		(AVALON_REG(AVALON_KEYS_OFFSET))
#define AVALON_SWITCHS_REG      	(AVALON_REG(AVALON_SWITCHS_OFFSET))
#define AVALON_LEDS_REG         	(AVALON_REG(AVALON_LEDS_OFFSET))
#define AVALON_HEX_REG          	(AVALON_REG(AVALON_HEX_OFFSET))
//-------------------
#define AVALON_ITP_STATUS_REG		(AVALON_REG(AVALON_ITP_STATUS_OFFSET))
#define AVALON_ITP_CLEAR_REG		(AVALON_REG(AVALON_ITP_CLEAR_OFFSET))
#define AVALON_ITP_MASK_REG			(AVALON_REG(AVALON_ITP_MASK_OFFSET))
//-------------------
#define AVALON_COUNTER_START_REG	(AVALON_REG(AVALON_COUNTER_START_OFFSET))
#define AVALON_COUNTER_STOP_REG		(AVALON_REG(AVALON_COUNTER_STOP_OFFSET))
#define AVALON_COUNTER_DELTA_REG	(AVALON_REG(AVALON_COUNTER_DELTA_OFFSET))
#define AVALON_COUNTER_ERROR_COUNT_REG	(AVALON_REG(AVALON_COUNTER_ERROR_COUNT_OFFSET))
#define AVALON_COUNTER_TRYS_COUNT_REG	(AVALON_REG(AVALON_COUNTER_TRYS_COUNT_OFFSET))
//-------------------
#define AVALON_COUNTER_CYCLE_COUNT_REG	(AVALON_REG(AVALON_COUNTER_CYCLE_COUNT_OFFSET))
//-------------------
#define AVALON_COUNTER_SPT_START_REG	(AVALON_REG(AVALON_COUNTER_SPT_START_OFFSET))
#define AVALON_COUNTER_SPT_STOP_REG		(AVALON_REG(AVALON_COUNTER_SPT_STOP_OFFSET))
#define AVALON_COUNTER_SPT_SETPOINT_REG	(AVALON_REG(AVALON_COUNTER_SPT_SETPOINT_OFFSET))
#define AVALON_COUNTER_SPT_FINISHED_REG	(AVALON_REG(AVALON_COUNTER_SPT_FINISHED_OFFSET))
//-------------------
#define AVALON_MX10_STATUS_REG		(AVALON_REG(AVALON_MX10_STATUS_OFFSET))
#define AVALON_MX10_TX_BUSY_REG		(AVALON_REG(AVALON_MX10_TX_BUSY_OFFSET))
#define AVALON_MX10_CS_REG			(AVALON_REG(AVALON_MX10_CS_OFFSET))
#define AVALON_MX10_DATA_REG		(AVALON_REG(AVALON_MX10_DATA_OFFSET))
//-------------------
//-------------------------------------------------------



//------------------------MACROS-------------------------
//MACROS GET
#define AVALON_GET_VALUE(REG, VALUE_MASK, VALUE_SHIFT, IS_HIGH) \
    ({ \
        uint32_t temp = REG; \
        temp = (temp & VALUE_MASK) >> VALUE_SHIFT; \
        IS_HIGH ? (~temp & ((VALUE_MASK) >> VALUE_SHIFT)) : temp; \
    })
//MACROS SET
#define AVALON_SET_VALUE(REG, VALUE_MASK, VALUE_SHIFT, VALUE, IS_HIGH) \
    do { \
        uint32_t temp = REG; \
        temp &= ~VALUE_MASK; \
        uint32_t value_to_set = IS_HIGH ? (~VALUE & ((VALUE_MASK) >> VALUE_SHIFT)) : VALUE; \
        temp |= ((value_to_set << VALUE_SHIFT) & VALUE_MASK); \
        REG = temp; \
    } while (0)
//-------------------------------------------------------



//----------------------PROTOTYPES-----------------------
// Prototypes des fonctions (lecture)
uint32_t	read_user_id(void);
uint32_t 	read_keys(void);
uint32_t 	read_switchs(void);
uint32_t 	read_leds(void);
uint32_t	read_hex0(void);
uint32_t	read_hex1(void);
uint32_t	read_hex2(void);
uint32_t	read_hex3(void);
//-------------------
uint32_t	read_itp_status(void);
uint32_t	read_itp_mask(void);
//-------------------
uint32_t	read_counter_delta(void);
uint32_t	read_counter_error_count(void);
uint32_t	read_counter_trys_count(void);
//-------------------
uint32_t	read_counter_cycle_count(void);
//-------------------
uint32_t	read_counter_spt_finished(void);
//-------------------
uint32_t	read_max10_status(void);
uint32_t	read_max10_tx_busy(void);
uint32_t	read_max10_cs(void);
uint32_t	read_max10_data(void);
//-------------------


// Prototypes des fonctions (écriture)
void 		write_leds(uint32_t value);
void		write_hex0(uint32_t value, bool isInt);
void		write_hex1(uint32_t value, bool isInt);
void		write_hex2(uint32_t value, bool isInt);
void		write_hex3(uint32_t value, bool isInt);
//-------------------
void		write_itp_clear();
void		write_itp_mask(uint32_t value);
//-------------------
void        write_counteur_start(uint32_t value);
void        write_counteur_stop(uint32_t value);
//-------------------
void		write_counteur_spt_start(uint32_t value);
void		write_counteur_spt_stop(uint32_t value);
void		write_counteur_spt_setpoint(uint32_t value);
//-------------------
void        write_max10_cs(uint32_t value);
void        write_max10_data(uint32_t value);
//-------------------
//-------------------------------------------------------


#endif /* SRC_AVALON_AVALON_FUNCTIONS_H_ */
