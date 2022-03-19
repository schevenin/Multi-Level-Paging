#ifndef LEVEL_H
#define LEVEL_H

#include <stdio.h>
#include <stdint.h>
#include "pagetable.h"
#include "map.h"

struct Level
{
    struct PageTable *pageTable;

    int depth;         // level depth
    bool isLeaf;       // is leaf node flag
    
    Level **nextLevel; // array of level pointers (non-leaf level)
    Map *mappings;     // mappings for each virtual page number to a physical frame. (leaf level)
};

void pageInsert(Level *level, uint32_t virtualAddress, uint32_t frame); // inserts the page into the level

#endif