#include "pagetable.h"

void pageInsert(PageTable *pageTable, uint32_t address, uint32_t frame)
{
   pageInsert(pageTable->rootLevelPtr, address, frame);
}

Map *pageLookup() {
   return NULL;
}

uint32_t virtualAddressToPageNum(uint32_t virtualAddress, uint32_t mask, uint32_t shift)
{
   uint32_t result;
   result = mask & virtualAddress;
   result = result >> shift;
   return result;
}