#include "pagetable.h"

Map *pageLookup(PageTable *pageTable, uint32_t virtualAddress) {




   return NULL;
}

void pageInsert(PageTable *pageTable, uint32_t virtualAddress, uint32_t frame)
{

   // create root level
   Level *newLevel = new Level();
   newLevel->pageTable = pageTable;
   newLevel->depth = 0;

   int size = pageTable->entriesPerLevel[newLevel->depth];
   Level **nextLevel = new Level*[size];

   for (int i = 0; i < size; i++) {
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