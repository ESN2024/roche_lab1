#include <stdio.h>
#include "system.h"
#include <sys/alt_irq.h>
#include <io.h>
#include <alt_types.h>
#include "sys/alt_sys_init.h"
#include "altera_avalon_pio_regs.h"

static void irqhandler(void* context, alt_u32 id)
{
    // Lire et réinitialiser le registre de capture de bord pour effacer l'interruption
    IORD_ALTERA_AVALON_PIO_EDGE_CAP(PIO_1_BASE);
    IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_1_BASE, 0);

    // Action de réponse à l'interruption
    IOWR_ALTERA_AVALON_PIO_DATA(PIO_0_BASE, 0x02);
    usleep(100000);
}

int main()
{
    alt_printf("main() started\n");

    // Configurer le bouton pour générer des interruptions
    IOWR_ALTERA_AVALON_PIO_IRQ_MASK(PIO_1_BASE, 0x1);
    IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_1_BASE, 0x0);

    // Enregistrer l'interruption
    alt_irq_register(PIO_1_IRQ, NULL, irqhandler);

    // Boucle principale
    while(1) {
        // Logique principale ici (si nécessaire)
    }

    return 0;
}
