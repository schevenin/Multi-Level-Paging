#include "pagetable.h"

void pageInsert(PageTable *pageTable, uint32_t address, uint32_t frame)
{
   /* get next address and process */
      if (NextAddress(pageTable->tracefile, pageTable->trace))
      {
         createBitmaskPageNumber(pageTable);
         pageTable->virtualPageNumber = pageTable->bitmask[0] & pageTable->trace->addr;
         pageTable->virtualPageNumber = pageTable->virtualPageNumber >> pageTable->offsetSize;
         
         
         // frame++;
         
         //std::cout<<page->temp.frame<<std::endl;
         
         //std::cout << std::hex << pageTable->virtualPageNumber << std::endl;
         
         
         //report_virtual2physical(page->trace->addr, page->temp.frame);
         
         
         //std::cout<<pagetable->offset<<std::endl;
         //pagetable->LevelPtr->map.push_back(temp);
         
        pageTable->instructionsProcessed++; // ensures correct amount of addreesses are processed
      }
}

void createBitmaskPageNumber(PageTable *pageTable)
{
   for (size_t i = 0; i < pageTable->numLevels; i++)
   {
      pageTable->bitmask[i] = ((1 << pageTable->numbits[i]) - 1) << pageTable->offsetSize;

      // std::cout<<pagetable->numbits[i]<<std::endl;
   }
}

void createBitmaskOffset(PageTable *pageTable)
{
}