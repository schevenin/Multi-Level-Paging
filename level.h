#ifndef LEVEL_H
#define LEVEL_H

#include <stdio.h>
#include <stdint.h>
#include "pagetable.h"
#include "map.h"

struct Level{
int depth;
struct PageTable *pageTable;
bool isLeaf;
Level *nextLevel; //Array of Level* pointers to the next level. (non-leaf or interior level)
Map** map; //array of MAP objects, each mapping a logical/virtual page to a physical frame. (leaf level)
};

//void pageInsert(PageTable page, uint32_t address, uint32_t frame); //inserts the page into the level

#endif