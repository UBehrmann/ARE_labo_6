/*
 * uart0.h
 *
 *  Created on: Dec 20, 2024
 *      Author: reds
 */

#ifndef SRC_UART_UART_H_
#define SRC_UART_UART_H_


//----INCLUDES
#include <stdint.h>


//----BASE ADDRESS
#define UART0_BASE_ADD 			0xFFC02000
#define UART1_BASE_ADD 			0xFFC03000


//----ADDRES'S OFFSETS
#define UART_DLL_OFFSET     	0x00 // Registre du latch bas du diviseur
#define UART_RBR_OFFSET			0x00 // Registre du buffer de réception
#define UART_THR_OFFSET			0x00 // Registre du buffer de transmission

#define UART_DLH_OFFSET     	0x04 // Registre d'activation des interruptions
#define UART_IER_OFFSET			0x04 // Registre du latch haut du diviseur
#define UART_IIR_OFFSET         0x08 // Registre d'identité des interruptions (lecture uniquement)
#define UART_FCR_OFFSET         0x08 // Registre de contrôle des FIFO (écriture uniquement)
#define UART_LCR_OFFSET         0x0C // Registre de contrôle de ligne
#define UART_MCR_OFFSET         0x10 // Registre de contrôle du modem
#define UART_LSR_OFFSET         0x14 // Registre d'état de ligne
#define UART_MSR_OFFSET         0x18 // Registre d'état du modem
#define UART_SCR_OFFSET         0x1C // Registre Scratchpad
#define UART_SRBR_OFFSET        0x30 // Registre tampon de réception shadow
#define UART_STHR_OFFSET        0x34 // Registre tampon de transmission shadow
#define UART_FAR_OFFSET         0x70 // Registre d'accès FIFO
#define UART_TFR_OFFSET         0x74 // Registre de lecture FIFO de transmission
#define UART_RFW_OFFSET         0x78 // Registre d'écriture FIFO de réception
#define UART_USR_OFFSET         0x7C // Registre d'état de l'UART
#define UART_TFL_OFFSET         0x80 // Niveau FIFO de transmission
#define UART_RFL_OFFSET         0x84 // Niveau FIFO de réception
#define UART_SRR_OFFSET         0x88 // Registre de réinitialisation logicielle
#define UART_SRTS_OFFSET        0x8C // Shadow du signal Request to Send (RTS)
#define UART_SBCR_OFFSET        0x90 // Shadow du contrôle de Break
#define UART_SDMAM_OFFSET       0x94 // Shadow du mode DMA
#define UART_SFE_OFFSET         0x98 // Shadow de l'activation des FIFO
#define UART_SRT_OFFSET         0x9C // Shadow du déclencheur de réception (Rx Trigger)
#define UART_STET_OFFSET        0xA0 // Shadow du déclencheur de FIFO vide en transmission
#define UART_HTX_OFFSET         0xA4 // Halt de la transmission
#define UART_DMASA_OFFSET       0xA8 // Accusé de réception logiciel pour le DMA
#define UART_CPR_OFFSET         0xF4 // Registre des paramètres du composant
#define UART_UCV_OFFSET         0xF8 // Version du composant
#define UART_CTR_OFFSET         0xFC // Type du composant


//----MACRO
#define UART_REG(base, offset) (*(volatile uint32_t *)((base) + (offset)))


//----PROTOTYPE
// Configuration du baud rate (100MHz horloge, 9600 baudrate)
void configure_baud_rate(uint32_t base, uint32_t clock_freq, uint32_t baudrate);
void configure_line(uint32_t base);
void enable_fifo(uint32_t base);
void write_char_uart(uint32_t base, uint8_t data);
void write_str_uart(uint32_t base, const char* data);



#endif /* SRC_UART_UART_H_ */
