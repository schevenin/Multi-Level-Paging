#include "pagetable.h"

void pageInsert(PageTable *page, uint32_t address, uint32_t frame)
{
   while (!feof(page->tracefile) && page->instructionsProcessed != page->numberofAddresses)
   {
      /* get next address and process */
      if (NextAddress(page->tracefile, page->trace))
      {

         createBitmaskPageNumber(page);
         page->temp.frame = page->bitmask[0] & address;
         frame++;
         int Page = (page->bitmask[0] &address);
         Page = Page >> page->offset;
         //std::cout<<page->temp.frame<<std::endl;

         //std::cout<<pagetable->offset<<std::endl;
         //pagetable->LevelPtr->map.push_back(temp);
         
        page->instructionsProcessed++; // ensures correct amount of addreesses are processed
      }
   }
};

void createBitmaskPageNumber(PageTable *page)
{

   for (size_t i = 0; i < page->numLevels; i++)
   {
      page->bitmask[i] = ((1 << page->numbits[i]) - 1) << page->offset;
      std::cout << page->bitmask[i] << std::endl;

      // std::cout<<pagetable->numbits[i]<<std::endl;
   }
}

void createBitmaskOffset(PageTable *page)
{
}