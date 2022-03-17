#include <stdio.h>
#include <stdint.h>
#include "pagetable.h"

struct Map {
   // bool oldestpage; //keeps track of the oldest existing page
   // bool mostRecent; //most recent page
    uint32_t frame; //physical frame
    uint32_t vpn; // virtual page number
};
