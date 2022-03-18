#include "pagetable.h"

void pageInsert(Level *level, uint32_t address, uint32_t frame){
    int currentPageLevel = level->depth;
    if (level->isLeaf) {
        // set appropriate page index to valid and store Frame
    } else {
        // create new level and set depth to current depth + 1
        Level *newLevel = new Level();
        newLevel->depth = level->depth+1;

        // Create an array of Level * entries based upon the number of entries in the new level and initialize to null/invalid as appropriate
        Level **nextLevel;

        //pageInsert(pointer to new Level, address, frame);
    }
}