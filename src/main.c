#include <gba.h>
#include <stdio.h>

int main(void)
{
    irqInit();
    irqEnable(IRQ_VBLANK);

    consoleDemoInit();

    iprintf("Hello, ME!");

    while (1)
    {
        VBlankIntrWait();
    }
    return 0;
}