#include "pagetable.h"

/**
 * @brief Recursively performs an insertion of a VPN->VPN mapping in PageTable
 * 
 * @param pageTable Level to insert from
 * @param virtualAddress Virtual address to insert
 * @param newFrame New frame to assign the inserted virtual address
 */
void pageInsert(Level *level, uint32_t virtualAddress, uint32_t newFrame)
{
    int currentDepth = level->depth;
    uint32_t indexToInsert = level->pageTable->pageLookup[currentDepth];
    uint32_t vpn = virtualAddressToPageNum(virtualAddress, level->pageTable->vpnMask, level->pageTable->offsetSize);

    // if leaf node
    if (currentDepth == (level->pageTable->numLevels) - 1)
    {
        level->isLeaf = true; // set level as leaf

        // if mappings in level are all NULL
        if (level->mappings == NULL)
        {
            level->mappings = new Map[level->pageTable->entriesPerLevel[currentDepth]]; // create new array of mappings of appropriate size

            // initialize new mappings array to invalid -1's
            // ensures that vpn 0/NULL isn't mistakenly found
            for (int i = 0; i < level->pageTable->entriesPerLevel[currentDepth]; i++)
            {
                level->mappings[i].vpn = -1;
                level->mappings[i].frame = -1;
            }
        }

        // insert VPN->PFN mapping at appropriate index
        level->mappings[indexToInsert].vpn = vpn;
        level->mappings[indexToInsert].frame = newFrame;
    }
    // is not leaf node
    else
    {
        // if entry already exists at index
        if (level->nextLevel[indexToInsert] != NULL)
        {
            // continue onto next level
            pageInsert(level->nextLevel[indexToInsert], virtualAddress, newFrame);
        }
        // if entry doesn't exist at index
        else
        {
            // if entire next levels are NULL
            if (level->nextLevel == NULL)
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
                nextLevel[i] = NULL;
            }

            newLevel->nextLevel = nextLevel;            // assign NULL entries to new level
            level->nextLevel[indexToInsert] = newLevel; // assign current level with pointer to the next

            pageInsert(newLevel, virtualAddress, newFrame); // insert new level
        }
    }
}