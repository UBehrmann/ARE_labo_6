/*
 * avalon.h
 *
 *  Created on: Dec 20, 2024
 *      Author: reds
 */

#ifndef SRC_AVALON_AVALON_H_
#define SRC_AVALON_AVALON_H_


// Includes
#include "../axi_lw.h"


// Base address
#define AVALON_OFFSET		0x010000
#define AVALON_BASE_ADD     (AXI_LW_HPS_FPGA_BASE_ADD + AVALON_OFFSET)


// ACCESS MACROS
#define AVALON_REG(_x_)   *(volatile uint32_t *)(AVALON_BASE_ADD + _x_) // _x_ is an offset with respect to the base address


#endif /* SRC_AVALON_AVALON_H_ */
