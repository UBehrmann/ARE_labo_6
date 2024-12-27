/*****************************************************************************************
 * HEIG-VD
 * Haute Ecole d'Ingenerie et de Gestion du Canton de Vaud
 * School of Business and Engineering in Canton de Vaud
 *****************************************************************************************
 * REDS Institute
 * Reconfigurable Embedded Digital Systems
 *****************************************************************************************
 *
 * File                 : execptions.c
 * Author               : Anthony Convers
 * Date                 : 27.10.2022
 *
 * Context              : ARE lab
 *
 *****************************************************************************************
 * Brief: defines exception vectors for the A9 processor
 *        provides code that sets the IRQ mode stack, and that dis/enables interrupts
 *        provides code that initializes the generic interrupt controller
 *
 *****************************************************************************************
 * Modifications :
 * Ver    Date        Engineer      Comments
 * 0.0    27.10.2022  ACS           Initial version.
 *
*****************************************************************************************/
#include <stdint.h>

#include "address_map_arm.h"
#include "int_defines.h"
#include "exceptions.h"
/* This file:
 * 1. defines exception vectors for the A9 processor
 * 2. provides code that sets the IRQ mode stack, and that dis/enables interrupts
 * 3. provides code that initializes the generic interrupt controller
*/

// Define the IRQ exception handler
void __attribute__ ((interrupt)) __cs3_isr_irq (void)
{
    // Lire l'ID de l'interruption
    uint32_t interrupt_id = *(volatile uint32_t *)GIC_ICCIAR_INTERRUPT_ACK;

    // Vérifier si l'interruption provient du timer
    if (interrupt_id == FPGA_IRQ_0_ID) { // ID du Timer HPS
    	fpga_ISR();
    }

    // Signaler la fin de l'interruption
    *(volatile uint32_t *)GIC_ICCEOIR_END_OF_INTERRUPT = interrupt_id;
}


// Define the remaining exception handlers
void __attribute__ ((interrupt)) __cs3_reset (void)
{
    while(1);
}

void __attribute__ ((interrupt)) __cs3_isr_undef (void)
{
    while(1);
}

void __attribute__ ((interrupt)) __cs3_isr_swi (void)
{
    while(1);
}

void __attribute__ ((interrupt)) __cs3_isr_pabort (void)
{
    while(1);
}

void __attribute__ ((interrupt)) __cs3_isr_dabort (void)
{
    while(1);
}

void __attribute__ ((interrupt)) __cs3_isr_fiq (void)
{
    while(1);
}

/* 
 * Initialize the banked stack pointer register for IRQ mode
*/
void set_A9_IRQ_stack(void)
{
	uint32_t stack, mode;
	stack = A9_ONCHIP_END - 7;		// top of A9 onchip memory, aligned to 8 bytes
	/* change processor to IRQ mode with interrupts disabled */
	mode = INT_DISABLE | IRQ_MODE;
	asm("msr cpsr, %[ps]" : : [ps] "r" (mode));
	/* set banked stack pointer */
	asm("mov sp, %[ps]" : : [ps] "r" (stack));

	/* go back to SVC mode before executing subroutine return! */
	mode = INT_DISABLE | SVC_MODE;
	asm("msr cpsr, %[ps]" : : [ps] "r" (mode));
}

/* 
 * Turn on interrupts in the ARM processor
*/
void enable_A9_interrupts(void)
{
	uint32_t status = SVC_MODE | INT_ENABLE;
	asm("msr cpsr, %[ps]" : : [ps]"r"(status));
}

/* Configure the Generic Interrupt Controller (GIC)
*
* - Le GIC (Generic Interrupt Controller) gère, priorise, et route les interruptions
* - des périphériques vers les cœurs du processeur
*/
void config_GIC(void) {
   // Configurer l'interruption pour le Timer HPS (ID 29 - à vérifier dans la documentation)
   config_interrupt(FPGA_IRQ_0_ID, CPU0);

   // Régler le Priority Mask Register pour autoriser toutes les priorités
   *(int *)GIC_ICCPMR_PRIORITY_MASK = 0xFFFF;

   // Activer l'interface CPU
   *(int *)GIC_ICCICR_INTERFACE_CTRL = ENABLE;

   // Activer le distributeur
   *(int *)GIC_ICDDCR_DISTRIBUTOR_CTRL = ENABLE;
}

/*
* Configure une interruption spécifique dans le GIC
* N : ID de l'interruption
* CPU_target : Cible CPU (par ex. CPU0)
*/
void config_interrupt(int N, int CPU_target) {
   int reg_offset, index, value, address;

   // Configurer les Set-Enable Registers (ICDISERn)
   reg_offset = (N >> 5) * 4; // Calculer l'offset du registre (N / 32) * 4
   index = N & 0x1F;          // Obtenir l'index du bit dans le registre
   value = 0x1 << index;      // Créer le masque pour l'interruption
   address = GIC_ICDISERn_INTERRUPT_SET_ENALBE + reg_offset; // Base pour ICDISERn
   *(int *)address |= value;  // Activer l'interruption

   // Configurer les Processor Targets Registers (ICDIPTRn)
   reg_offset = (N & 0xFFFFFFFC);  // Offset du registre (N / 4) * 4
   index = N & 0x3;               // Obtenir l'index du byte
   address = GIC_ICDIPTRn_INTERRUPT_PROCESSOR_TARGETS + reg_offset + index; // Base pour ICDIPTRn
   *(char *)address = (char)CPU_target; // Associer l'interruption au CPU
}
