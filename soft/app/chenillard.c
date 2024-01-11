#include <stdio.h>
#include "system.h"
#include "altera_avalon_pio_regs.h"
#include "sys/alt_stdio.h"
#include <unistd.h>

int main() {
    unsigned int data = 0x01; 
    while(1) {
        while(data!=0x100) {
            IOWR_ALTERA_AVALON_PIO_DATA(PIO_0_BASE,data);
            usleep(100000);
            data = data << 1;
        }
        data = 0x01;
    }
    return 0;
}
