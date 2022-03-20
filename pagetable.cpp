#include "pagetable.h"

Map *pageLookup(PageTable *pageTable, uint32_t virtualAddress)
{
   uint32_t vpnKey = virtualAddress;              // looking for in PageTable
   Level *currentLevel = pageTable->rootLevelPtr; // start search at root

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
               // if vpn == key
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

void pageInsert(PageTable *pageTable, uint32_t virtualAddress, uint32_t frame)
{
   // if a root level already exists
   if (pageTable->rootLevelPtr != NULL)
   {
      // continue onto root level
      pageInsert(pageTable->rootLevelPtr, virtualAddress, frame);
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
      pageInsert(newLevel, virtualAddress, frame); // insert root level
   }
}

uint32_t virtualAddressToPageNum(uint32_t virtualAddress, uint32_t mask, uint32_t shift)
{
   uint32_t result;
   result = (virtualAddress & mask) >> shift;
   return result;
}

int countPageTableSize(PageTable *pageTable, Level *level)
{
   int sum = 0;
   sum += sizeof(level);

   int entryCount = pageTable->entriesPerLevel[level->depth];
   sum += entryCount * 8; // bits to bytes
   for (int i = 0; i < entryCount; i++)
   {
      if (level->nextLevel[i] != NULL)
      {
         sum += countPageTableSize(pageTable, level->nextLevel[i]);
      }
   }

   sum += sizeof(pageTable);
   return sum;
}