// boot_alim / Gestion de shutdown/wakeup //

#include <pic18fregs.h>
#include "common_types.h"
#include "pindebug.h"

uchar memory = 0;

void boot_suspend(void)
{
    memory = PINDEBUG;
}

void boot_wakeup(void)
{
    PINDEBUG = memory;
}
