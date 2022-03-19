#include "pagetable.h"

void pageInsert(Level *level, uint32_t address, uint32_t frame){
    int currentLevel = level->depth;
    uint32_t index = level->pageTable->pageLookup[currentLevel];

    // if leaf node
    if (currentLevel == level->pageTable->numLevels-1) {

        level->mappings = new Map[level->pageTable->entriesPerLevel[level->pageTable->numLevels]];

        level->mappings[index].vpn = address;

        level->mappings[index].frame = frame;

        std::cout << "created mapping (level " << currentLevel << "): " << level->mappings[index].vpn << " -> " << level->mappings[index].vpn << std::endl;
    } else {
        // create new level and set depth to current depth + 1
        Level *newLevel = new Level();
        newLevel->pageTable = level->pageTable;
        newLevel->depth = level->depth+1;

        // Create an array of Level* entries based upon the number of entries in the new level and initialize to null/invalid as appropriate
        int size = level->pageTable->entriesPerLevel[newLevel->depth];
        Level **nextLevel = new Level*[size];

        for (int i = 0; i < size; i++) {
            nextLevel[i] = NULL;
        }

        // assign array of level pointers to the new level
        newLevel->nextLevel = nextLevel;

        // assign current level with pointer to the next
        level->nextLevel[index] = newLevel;

        //pageInsert(pointer to new Level, address, frame);
        pageInsert(newLevel, address, frame);
    }
}