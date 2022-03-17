#include "pagetable.h"

void pageInsert(PageTable *page, uint32_t address, uint32_t frame)
{
   /* get next address and process */
      if (NextAddress(page->tracefile, page->trace))
      {
         createBitmaskPageNumber(page);
         page->virtualPageNumber = page->bitmask[0] & page->trace->addr;
         page->virtualPageNumber = page->virtualPageNumber >> page->offsetSize;
         
         
         // frame++;
         
         //std::cout<<page->temp.frame<<std::endl;
         
         std::cout << std::hex << page->virtualPageNumber << std::endl;
         
         
         //report_virtual2physical(page->trace->addr, page->temp.frame);
         
         
         //std::cout<<pagetable->offset<<std::endl;
         //pagetable->LevelPtr->map.push_back(temp);
         
        page->instructionsProcessed++; // ensures correct amount of addreesses are processed
      }
}

void createBitmaskPageNumber(PageTable *page)
{
   for (size_t i = 0; i < page->numLevels; i++)
   {
      page->bitmask[i] = ((1 << page->numbits[i]) - 1) << page->offsetSize;

      // std::cout<<pagetable->numbits[i]<<std::endl;
   }
}

void createBitmaskOffset(PageTable *page)
{
}