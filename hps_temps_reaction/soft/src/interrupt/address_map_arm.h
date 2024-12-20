/*****************************************************************************************
 * HEIG-VD
 * Haute Ecole d'Ingenerie et de Gestion du Canton de Vaud
 * School of Business and Engineering in Canton de Vaud
 *****************************************************************************************
 * REDS Institute
 * Reconfigurable Embedded Digital Systems
 *****************************************************************************************
 *
 * File                 : address_map_arm.h
 * Author               : Anthony Convers
 * Date                 : 27.10.2022
 *
 * Context              : ARE lab
 *
 *****************************************************************************************
 * Brief: provides address values that exist in the ARM system
 *
 *****************************************************************************************
 * Modifications :
 * Ver    Date        Engineer      Comments
 * 0.0    27.10.2022  ACS           Initial version.
 *
*****************************************************************************************/

#define BOARD                 			"DE1-SoC"

// Memory
#define DDR_BASE              			0x00000000
#define DDR_END               			0x3FFFFFFF
#define A9_ONCHIP_BASE        			0xFFFF0000
#define A9_ONCHIP_END         			0xFFFFFFFF
#define SDRAM_BASE            			0xC0000000
#define SDRAM_END             			0xC3FFFFFF
#define FPGA_ONCHIP_BASE      			0xC8000000
#define FPGA_ONCHIP_END       			0xC803FFFF
#define FPGA_CHAR_BASE        			0xC9000000
#define FPGA_CHAR_END         			0xC9001FFF

// GIC Base Addresses
#define GIC_ICCPMR_PRIORITY_MASK					0xFFFEC104
#define GIC_ICCICR_INTERFACE_CTRL					0xFFFEC100
#define GIC_ICDDCR_DISTRIBUTOR_CTRL					0xFFFED000
#define GIC_ICDISERn_INTERRUPT_SET_ENALBE			0xFFFED100
#define GIC_ICDIPTRn_INTERRUPT_PROCESSOR_TARGETS	0xFFFED800
#define GIC_ICCIAR_INTERRUPT_ACK					0xFFFEC10C
#define GIC_ICCEOIR_END_OF_INTERRUPT				0xFFFEC110

// Timer Interrupt ID
#define FPGA_IRQ_0_ID 							72
