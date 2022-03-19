#include "pagetable.h"

void pageInsert(Level *level, uint32_t address, uint32_t frame)
{
    int currentLevel = level->depth;
    uint32_t index = level->pageTable->pageLookup[currentLevel];

    // if leaf node
    if (currentLevel == (level->pageTable->numLevels) - 1)
    {
        level->isLeaf = true;

        // if mappings in level is NULL
        if (level->mappings == NULL)
        {
            level->mappings = new Map[level->pageTable->entriesPerLevel[level->pageTable->numLevels - 1]];
        }

        // add mapping
        level->mappings[index].vpn = address;
        level->mappings[index].frame = frame;
    }
    // is not leaf node
    else
    {
        // if next level is null
        if (level->nextLevel[index] == NULL)
        {

            // create new level and set depth to current depth + 1
            Level *newLevel = new Level();
            newLevel->pageTable = level->pageTable;
            newLevel->depth = level->depth + 1;

            // array of level* entries based upon the number of entries in the new level
            int size = level->pageTable->entriesPerLevel[newLevel->depth];
            Level **nextLevel = new Level *[size];

            // initialize next level to NULL
            for (int i = 0; i < size; i++)
            {
                nextLevel[i] = NULL;
            }

            // assign array of level pointers to the new level
            newLevel->nextLevel = nextLevel;

            // assign current level with pointer to the next
            level->nextLevel[index] = newLevel;

            // insert new level
            pageInsert(newLevel, address, frame);
        }
        else
        {
            pageInsert(level->nextLevel[index], address, frame);
        }
    }
}