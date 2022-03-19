#include "pagetable.h"

Map *pageLookup(PageTable *pageTable, uint32_t virtualAddress)
{
   uint32_t vpnKey = pageTable->vpn; // looking for in PageTable
   Level *currentLevel = pageTable->rootLevelPtr; // start search at root

   // for each level
   for (int x = 0; x < pageTable->numLevels; x++)
   {
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
               // if the vpn is equal to the key
               if (currentLevel->mappings[index].vpn == vpnKey)
               {
                  // return the pair
                  Map *pair = new Map();
                  pair->vpn = currentLevel->mappings[index].vpn;
                  pair->frame = currentLevel->mappings[index].frame;
                  return pair;
               }
            }
         }
         else
         {
            // not a leaf level, move down a level
            currentLevel = currentLevel->nextLevel[index];
         }
      }
   }

   return NULL;
}

void pageInsert(PageTable *pageTable, uint32_t virtualAddress, uint32_t frame)
{
   // create root level
   Level *newLevel = new Level();
   pageTable->rootLevelPtr = newLevel;

   newLevel->pageTable = pageTable;
   newLevel->depth = 0;

   int size = pageTable->entriesPerLevel[newLevel->depth];
   Level **nextLevel = new Level *[size];

   for (int i = 0; i < size; i++)
   {
      nextLevel[i] = NULL;
   }

   newLevel->nextLevel = nextLevel;

   pageInsert(newLevel, virtualAddress, frame);
}

uint32_t virtualAddressToPageNum(uint32_t virtualAddress, uint32_t mask, uint32_t shift)
{
   uint32_t result;
   result = (virtualAddress & mask) >> shift;
   return result;
}