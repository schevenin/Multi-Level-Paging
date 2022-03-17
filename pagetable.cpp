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
         pageTable->virtualPageNumber = virtualAddressToPageNum(pageTable->trace->addr, pageTable->bitmask[i], (pageTable->bitshift[i]));

       //  pageTable->virtualPageNumber = pageTable->bitmask[i] & pageTable->trace->addr;
       //  pageTable->virtualPageNumber = pageTable->virtualPageNumber >> (pageTable->bitshift[i]);
        // std::cout << "Masked Result (" << i << "): " << (pageTable->virtualPageNumber) << std::endl;

         // frame++;
         
         //std::cout<<page->temp.frame<<std::endl;
         
         std::cout << "Virtual Page Number at Level ("<< i<< ") "<< pageTable->virtualPageNumber << std::endl;
         
         
         //report_virtual2physical(page->trace->addr, page->temp.frame);
         
         
         //std::cout<<pagetable->offset<<std::endl;
         //pagetable->LevelPtr->map.push_back(temp);
         
         pageTable->instructionsProcessed++; // ensures correct amount of addreesses are processed
      }
   }
   
}

void createBitmaskPageNumber(PageTable *pageTable)
{
  // std::cout << "Creating VPN mask:" << std::endl;
   for (int i = 0; i < pageTable->numLevels; i++)
   {
      pageTable->bitmask[i] = (1 << pageTable->numBits[i]) - 1;
      //pageTable->bitmask[i] = pageTable->bitmask[i]  pageTable->bitsProcessed;

      pageTable->bitmask[i] = pageTable->bitmask[i] << (pageTable->bitshift[i]);
      
     // pageTable->bitmask[i] = pageTable->bitmask[i] >> pageTable->bitsProcessed;
     // std::cout<<(32-pageTable->bitsProcessed);
      
      //pageTable->bitmask[i] = pageTable->bitmask[i]  (pageTable->bitsProcessed);
      //pageTable->bitmask[i] = pageTable->bitmask[i] << (32-pageTable->bitsProcessed);
    //  std::cout << pageTable->numBits[i] << std::dec;
      //std::cout << std::endl;
      //std::cout << "Bits (Level: " << i << "): " << std::dec << pageTable->numBits[i] << std::endl;
     std::cout << "Bitmask (Level: " << i << "): " << std::hex << pageTable->bitmask[i] << std::endl;
   }
   std::cout << std::endl;
}

void createBitmaskOffset(PageTable *pageTable)
{

   pageTable->offsetMask = (1 << pageTable->offsetSize) - 1;
   //std::cout << std::hex<<pageTable ->offsetMask <<std::endl;
}

uint32_t virtualAddressToPageNum(uint32_t virtualAddress, uint32_t mask, uint32_t shift) {             // Example value: 0x01020304
   // uint32_t byte1 = (value >> 24);           // 0x01020304 >> 24 is 0x01 so
   //                                           // no masking is necessary
   // uint32_t byte2 = (value >> 16) & 0xff;    // 0x01020304 >> 16 is 0x0102 so
   //                                           // we must mask to get 0x02
   // uint32_t byte3 = (value >> 8)  & 0xff;    // 0x01020304 >> 8 is 0x010203 so
   //                                           // we must mask to get 0x03
   // uint32_t byte4 = value & 0xff;            // here we only mask, no shifting
   
   uint32_t result;
   result = mask & virtualAddress;
   result = result >> shift;

   return result;
}