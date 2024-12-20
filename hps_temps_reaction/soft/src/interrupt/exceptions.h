/*
 * exceptions.h
 *
 *  Created on: Dec 20, 2024
 *      Author: reds
 */
#ifndef SRC_EXCEPTIONS_H_
#define SRC_EXCEPTIONS_H_



#include <stdint.h>



//------------------------ Fonctions Utilitaires ------------------------
// Configurer la pile pour le mode IRQ
void set_A9_IRQ_stack(void);
// Activer les interruptions dans le processeur ARM
void enable_A9_interrupts(void);
// Configurer le GIC (Generic Interrupt Controller)
void config_GIC(void);
// Configurer une interruption spécifique dans le GIC
void config_interrupt(int N, int CPU_target);



#endif /* SRC_EXCEPTIONS_H_ */
