/**
 * @file pagetable.cpp
 * @author Rogelio Schevenin, Sawyer Thompson
 * @redID 824107681, 823687079
 * @brief Implements PageTable functions
 * @date 2022-03-21
 */

#include "pagetable.h"

/**
 * @brief Recursively performs a lookup of a VPN->PFN mapping in PageTable
 * 
 * @param pageTable PageTable object
 * @param virtualAddress Virtual address to be searched for.
 * @return Map* Mapping of found VPN->PFN
 */
Map *pageLookup(PageTable *pageTable, uint32_t virtualAddress)
{
   uint32_t vpnKey = virtualAddressToPageNum(virtualAddress, pageTable->vpnMask, pageTable->offsetSize); // looking for in PageTable
   Level *currentLevel = pageTable->rootLevelPtr;                                                        // start search at root

   // for each level in PageTable
   for (int x = 0; x < pageTable->numLevels; x++)
   {
      // index of next level/map entry
      uint32_t index = pageTable->pageLookup[x]; // get index for each level

      // if the level isn't empty
      if (currentLevel != NULL)
      {
         // is leaf level
         if (currentLevel->depth == pageTable->numLevels - 1)
         {
            // if level contains mappings
            if (currentLevel->mappings != NULL)
            {
               // if vpnKey is found
               if (currentLevel->mappings[index].vpn == vpnKey)
               {
                  return &currentLevel->mappings[index];
               }
            }
         }
         // not a leaf level
         else
         {
            // move to next level
            currentLevel = currentLevel->nextLevel[index];
         }
      }
   }

   return NULL;
}

/**
 * @brief Recursively performs an insertion of a VPN->VPN mapping in PageTable
 * 
 * @param pageTable PageTable object
 * @param virtualAddress Virtual address to insert
 * @param newFrame New frame to assign the inserted virtual address
 */
void pageInsert(PageTable *pageTable, uint32_t virtualAddress, uint32_t newFrame)
{
   // if a root level already exists
   if (pageTable->rootLevelPtr != NULL)
   {
      // continue onto root level
      pageInsert(pageTable->rootLevelPtr, virtualAddress, newFrame);
   }
   // root level doesn't exist
   else
   {
      Level *newLevel = new Level();   // create root level
      newLevel->pageTable = pageTable; // assign PageTable to root level
      newLevel->depth = 0;             // assign root depth

      // array of level* entries based upon the number of entries in the new level
      int size = pageTable->entriesPerLevel[newLevel->depth];
      Level **nextLevel = new Level *[size];

      // initialize next level entries to NULL
      for (int i = 0; i < size; i++)
      {
         nextLevel[i] = NULL;
      }

      newLevel->nextLevel = nextLevel;             // assign NULL entries to new level
      pageTable->rootLevelPtr = newLevel;          // assign pointer to root level in PageTable
      pageInsert(newLevel, virtualAddress, newFrame); // insert root level
   }
}

/**
 * @brief Applies masking and shifting of a given address
 * 
 * @param virtualAddress Virtual address to mask and shift
 * @param mask Mask (&) to apply to virtual address
 * @param shift Amount of bits to shift virtual address
 * @return uint32_t Masked and shifted result
 */
uint32_t virtualAddressToPageNum(uint32_t virtualAddress, uint32_t mask, uint32_t shift)
{
   uint32_t result;
   result = (virtualAddress & mask) >> shift;
   return result;
}

/**
 * @brief Recursively counts the size in bytes of a PageTable
 * 
 * @param pageTable PageTable object to count
 * @param level Level to begin counting
 * @return int Total count in bytes
 */
uint32_t countPageTableSize(PageTable *pageTable, Level *level)
{
   uint32_t sum = 0;
   sum += sizeof(level); // get size of current level

   uint32_t entryCount = pageTable->entriesPerLevel[level->depth]; // possible number of entries in current level
   sum += entryCount * 8; // regardless of exterior or interior level, add entries in bytes

   // for every possible entry in level
   for (int i = 0; i < entryCount; i++)
   {
      // get the size of the next level if it exists
      if (level->nextLevel[i] != NULL) {
         sum += countPageTableSize(pageTable, level->nextLevel[i]);
      }
   }

   // if last iteration, add the size of the page table
   if (level->isLeaf)
   {
      sum += sizeof(pageTable);
   }

   return sum;
}