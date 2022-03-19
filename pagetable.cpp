#include "pagetable.h"

Map *pageLookup(PageTable *pageTable, uint32_t virtualAddress) {

   


   return NULL;
}

void pageInsert(PageTable *pageTable, uint32_t virtualAddress, uint32_t frame)
{
   pageInsert(pageTable->rootLevelPtr, virtualAddress, frame);
}

uint32_t virtualAddressToPageNum(uint32_t virtualAddress, uint32_t mask, uint32_t shift)
{
   uint32_t result;
   result = (virtualAddress & mask) >> shift;
   return result;
}