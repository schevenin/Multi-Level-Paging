#include <stdio.h>
#include <stdint.h>
#include "pagetable.h"

struct Map {
    uint32_t frame; //physical frame
    uint32_t vpn; // virtual page number
};
