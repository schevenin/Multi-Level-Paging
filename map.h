#include <stdio.h>
#include <stdint.h>
#include "pagetable.h"

struct Map {
    int frame; //physical frame
    uint32_t vpn; // virtual page number
};
