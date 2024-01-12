#include <stdio.h>
#include "system.h"
#include <sys/alt_irq.h>
#include <io.h>
#include <alt_types.h>
#include "sys/alt_sys_init.h"
#include "altera_avalon_pio_regs.h"

volatile __uint8_t speed;
__uint8_t data = 0x01; 

static void irqhandler_bouton_key1(void* context, alt_u32 id)
{
    //IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_1_BASE, 0x1); ici on ne reinitialise pas pour garder l irq

    // Action de réponse à l'interruption
    if (data == 0x80){
        data = 0x01;
    }
    IOWR_ALTERA_AVALON_PIO_DATA(PIO_0_BASE,data);
    usleep(400000/speed);
    data = data << 1;
}

static void irqhandler_switch(void* context, alt_u32 id)
{

    // Action de réponse à l'interruption
    speed = IORD_ALTERA_AVALON_PIO_DATA(PIO_2_BASE) + 1; //+1 pour eviter le speed=0
    alt_printf("pio = %x\n", IORD_ALTERA_AVALON_PIO_DATA(PIO_2_BASE));
    alt_printf("speed = %x\n", speed);

    // Réinitialiser le registre de capture de bord pour effacer l'interruption
    IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_2_BASE, 0x0F);
}

int main()
{
    unsigned int data = 0x01; 
    alt_printf("main() started\n");
    speed = 0x01;
    // Configurer le bouton pour générer des interruptions
    IOWR_ALTERA_AVALON_PIO_IRQ_MASK(PIO_1_BASE, 0x1);
    IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_1_BASE, 0x1);

    // Configurer le switch pour générer des interruptions
    IOWR_ALTERA_AVALON_PIO_IRQ_MASK(PIO_2_BASE, 0x0F); //IRQ agit uniquement sur les 4 premier bits de la PIO_2
    IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_2_BASE, 0x0F);

    // Enregistrer l'interruption
    alt_irq_register(PIO_2_IRQ, NULL, irqhandler_switch);
    alt_irq_register(PIO_1_IRQ, NULL, irqhandler_bouton_key1);

    // Boucle principale
    while(1) {
        
    }

    return 0;
}
