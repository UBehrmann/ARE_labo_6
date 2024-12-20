/*****************************************************************************************
 * HEIG-VD
 * Haute Ecole d'Ingenerie et de Gestion du Canton de Vaud
 * School of Business and Engineering in Canton de Vaud
 *****************************************************************************************
 * REDS Institute
 * Reconfigurable Embedded Digital Systems
 *****************************************************************************************
 *
 * File                 : hps_application.c
 * Author               : 
 * Date                 : 
 *
 * Context              : ARE lab
 *
 *****************************************************************************************
 * Brief: Mesure du temps de reaction avec la carte DE1-SoC et MAX10
 *
 *****************************************************************************************
 * Modifications :
 * Ver    Date        Student      Comments
 * 
 *
*****************************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "axi_lw.h"
#include "uart/uart.h"

int __auto_semihosting;

int main(void){

    printf("Laboratoire: Mesurle du temps de reaction \n");
    
    // TO BE COMPLETE
    uint32_t uart_base = UART0_BASE_ADD;

    // Configurer le baudrate à 9600
    configure_baud_rate(uart_base, 100000000, 9600);

    // Configurer la ligne série (8 bits de données, pas de parité, 1 bit de stop)
    configure_line(uart_base);

    // Activer les FIFO
    enable_fifo(uart_base);

    // Envoyer un message
    const char *message = "Hello UART!";
    while (*message) {
        write_uart(uart_base, *message++);
    }

}
