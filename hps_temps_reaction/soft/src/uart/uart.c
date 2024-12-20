/*
 * uart.c
 *
 *  Created on: Dec 20, 2024
 *      Author: reds
 */
#include "uart.h"

// Configuration du baud rate (100MHz horloge, 9600 baudrate)
void configure_baud_rate(uint32_t base, uint32_t clock_freq, uint32_t baudrate) {
    uint16_t divisor = clock_freq / (16 * baudrate);

    // Activer l'accès aux registres DLL et DLH
    UART_REG(base, UART_LCR_OFFSET) |= (1 << 7); // DLAB = 1

    // Configurer les registres DLL et DLH avec le diviseur calculé
    UART_REG(base, UART_DLL_OFFSET) = divisor & 0xFF;        // Partie basse
    UART_REG(base, UART_DLH_OFFSET) = (divisor >> 8) & 0xFF; // Partie haute

    // Désactiver l'accès aux registres DLL et DLH
    UART_REG(base, UART_LCR_OFFSET) &= ~(1 << 7); // DLAB = 0
}

// Configuration des paramètres de ligne : 8 bits de données, pas de parité, 1 bit de stop
void configure_line(uint32_t base) {
    uint8_t lcr_config = 0x03; // 8 bits de données (bits 0-1 = 0b11), 1 bit de stop (bit 2 = 0), pas de parité (bit 3 = 0)
    UART_REG(base, UART_LCR_OFFSET) = lcr_config;
}

// Activer les FIFO en émission et réception
void enable_fifo(uint32_t base) {
    uint8_t fcr_config = 0x07; // Activer FIFO (bit 0), reset Rx FIFO (bit 1), reset Tx FIFO (bit 2)
    UART_REG(base, UART_FCR_OFFSET) = fcr_config;
}

// Vérification du buffer de transmission prêt
void write_uart(uint32_t base, uint8_t data) {
    // Attendre que le buffer de transmission soit vide
    while (!(UART_REG(base, UART_LSR_OFFSET) & (1 << 5)));
    // Envoyer les données
    UART_REG(base, UART_DLL_OFFSET) = data;
}

