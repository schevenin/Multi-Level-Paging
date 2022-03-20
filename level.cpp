#include "pagetable.h"

void pageInsert(Level *level, uint32_t address, uint32_t frame)
{
    int currentDepth = level->depth;
    uint32_t indexToInsert = level->pageTable->pageLookup[currentDepth];

    // if leaf node
    if (currentDepth == (level->pageTable->numLevels) - 1)
    {
        level->isLeaf = true; // set level as leaf

        // if mappings in level are all NULL
        if (level->mappings == nullptr)
        {
            level->mappings = new Map[level->pageTable->entriesPerLevel[currentDepth]]; // create new array of mappings of appropriate size
        }

        // insert VPN->PFN mapping at appropriate index
        level->mappings[indexToInsert].vpn = address;
        level->mappings[indexToInsert].frame = frame;
    }
    // is not leaf node
    else
    {
        // if entry already exists at index
        if (level->nextLevel[indexToInsert] != nullptr)
        {
            // continue onto next level
            pageInsert(level->nextLevel[indexToInsert], address, frame);
        }
        // if entry doesn't exist at index
        else
        {
            // if entire next levels are NULL
            if (level->nextLevel == nullptr)
            {
                level->nextLevel = new Level *[level->pageTable->entriesPerLevel[currentDepth]]; // create new array of mappings of appropriate size
            }

            Level *newLevel = new Level();          // create new level
            newLevel->pageTable = level->pageTable; // assign PageTable to new level
            newLevel->depth = level->depth + 1;     // assign new level depth + 1

            // array of level* entries based upon the number of entries in the new level
            int size = level->pageTable->entriesPerLevel[newLevel->depth];
            Level **nextLevel = new Level *[size];

            // initialize next level entries to NULL
            for (int i = 0; i < size; i++)
            {
                nextLevel[i] = nullptr;
            }

            newLevel->nextLevel = nextLevel;            // assign NULL entries to new level
            level->nextLevel[indexToInsert] = newLevel; // assign current level with pointer to the next

            pageInsert(newLevel, address, frame); // insert new level
        }
    }
}