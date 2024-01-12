#include <stdio.h>
#include "system.h"
#include <sys/alt_irq.h>
#include <io.h>
#include <alt_types.h>
#include "sys/alt_sys_init.h"
#include "altera_avalon_pio_regs.h"

static void irqhandler_bouton_key1(void* context, alt_u32 id)
{
    // Réinitialiser le registre de capture de bord pour effacer l'interruption
    IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_1_BASE, 0x1);

    // Action de réponse à l'interruption
    unsigned int data = 0x01; 
    while(1) {
        while(data!=0x100) {
            IOWR_ALTERA_AVALON_PIO_DATA(PIO_0_BASE,data);
            usleep(60000);
            data = data << 1;
        }
        data = 0x01;
    }
}


int main()
{
    alt_printf("main() started\n");

    // Configurer le bouton pour générer des interruptions
    IOWR_ALTERA_AVALON_PIO_IRQ_MASK(PIO_1_BASE, 0x1);
    IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_1_BASE, 0x1);


    // Enregistrer l'interruption
    alt_irq_register(PIO_1_IRQ, NULL, irqhandler_bouton_key1);

    // Boucle principale
    while(1) {
    }

    return 0;
}
