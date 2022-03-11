#include <stdio.h>
#include <stdint.h>
#include "pagetable.h"

struct Map{
    int oldestpage; //keeps track of the oldest existing page
    int mostRecent; //most recent page
    uint32_t address; //Virtual Adress
    uint32_t frame; //physical frame
};
