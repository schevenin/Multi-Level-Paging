#include "pagetable.h"

void pageInsert(PageTable *pageTable, uint32_t address, uint32_t frame)
{
   
 //  std::cout << "Address: " << std::hex << address << std::endl;
   createBitmaskOffset(pageTable);
   createBitmaskPageNumber(pageTable);

   for (int i = 0; i < pageTable->numLevels; i++) {
      /* get next address and process */
      if (NextAddress(pageTable->tracefile, pageTable->trace))
      {
         pageTable->virtualPageNumber = pageTable->bitmask[i] & pageTable->trace->addr;
         pageTable->virtualPageNumber = pageTable->virtualPageNumber >> pageTable->offsetSize;
        // std::cout << "Masked Result (" << i << "): " << (pageTable->virtualPageNumber) << std::endl;

         // frame++;
         
         //std::cout<<page->temp.frame<<std::endl;
         
         //std::cout << std::hex << pageTable->virtualPageNumber << std::endl;
         
         
         //report_virtual2physical(page->trace->addr, page->temp.frame);
         
         
         //std::cout<<pagetable->offset<<std::endl;
         //pagetable->LevelPtr->map.push_back(temp);
         
         pageTable->instructionsProcessed++; // ensures correct amount of addreesses are processed
      }
   }
   
}

void createBitmaskPageNumber(PageTable *pageTable)
{
   pageTable->bitsProcessed = 0;
  // std::cout << "Creating VPN mask:" << std::endl;
   for (int i = 0; i < pageTable->numLevels; i++)
   {
      pageTable->bitsProcessed += pageTable->numBits[i];
      pageTable->bitmask[i] = (1 << pageTable->numBits[i]) - 1;
      //pageTable->bitmask[i] = pageTable->bitmask[i]  pageTable->bitsProcessed;
      pageTable->bitmask[i] = pageTable->bitmask[i] << (32-pageTable->bitsProcessed);
      
     // pageTable->bitmask[i] = pageTable->bitmask[i] >> pageTable->bitsProcessed;
     // std::cout<<(32-pageTable->bitsProcessed);
      
      //pageTable->bitmask[i] = pageTable->bitmask[i]  (pageTable->bitsProcessed);
      //pageTable->bitmask[i] = pageTable->bitmask[i] << (32-pageTable->bitsProcessed);
    //  std::cout << pageTable->numBits[i] << std::dec;
      //std::cout << std::endl;
      //std::cout << "Bits (Level: " << i << "): " << std::dec << pageTable->numBits[i] << std::endl;
    //  std::cout << "Bitmask (Level: " << i << "): " << std::hex << pageTable->bitmask[i] << std::endl;
   }
   std::cout << std::endl;
}

void createBitmaskOffset(PageTable *pageTable)
{

   pageTable->offsetMask = (1 << pageTable->offsetSize) - 1;
   std::cout << std::hex<<pageTable ->offsetMask <<std::endl;
}