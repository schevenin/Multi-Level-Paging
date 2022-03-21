/**
 * @file level.h
 * @author Rogelio Schevenin, Sawyer Thompson
 * @redID 824107681, 823687079
 * @brief Header containing Level structure
 * @date 2022-03-21
 */

#ifndef LEVEL_H
#define LEVEL_H

#include "map.h"
#include "pagetable.h"

/**
 * @brief Level structure
 * Stores Level attributes
 */
struct Level
{
    struct PageTable *pageTable; // the PageTable that this level belongs to
    int depth;                   // level depth
    bool isLeaf;                 // is leaf node flag
    Level **nextLevel;           // array of level pointers (non-leaf level)
    Map *mappings;               // mappings for each virtual page number to a physical frame. (leaf level)
};

void pageInsert(Level *level, uint32_t virtualAddress, uint32_t frame); // inserts the page into the level

#endif