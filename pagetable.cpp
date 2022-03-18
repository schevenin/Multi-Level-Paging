#include "pagetable.h"

void pageInsert(PageTable *pageTable, uint32_t* address, uint32_t frame)
{
   /* get next address and process */
   if (NextAddress(pageTable->tracefile, pageTable->trace))
   {
      std::cout << "=================================" << std::endl;
      for (int i = 0; i < pageTable->numLevels; i++) {
         pageTable->virtualPageLookup = virtualAddressToPageNum(*address, pageTable->bitmask[i], pageTable->bitshift[i]) << pageTable->bitshift[i];
         pageTable->virtualPageNumber = virtualAddressToPageNum(*address, pageTable->vpnMask, pageTable->offsetSize) << pageTable->offsetSize;
         //pageTable->virtualPageNumber = virtualAddressToPageNum(*address, pageTable->bitmask[i], pageTable->bitshift[i]);
         //pageTable->offset = virtualAddressToPageNum(address, pageTable->bitmask[i], (pageTable->bitshift[i]));
         //fprintf(stdout, "\nLevel: %i\n", i);
         fprintf(stdout, "Virtual Address: %08X\n", address);

        fprintf(stdout, "\nOffset mask: %08X\n", pageTable->offsetMask);
         fprintf(stdout, "VP Lookup Mask (%i): %08X\n", i, pageTable->bitmask[i]);
         fprintf(stdout, "\nVPN Mask (%i): %X\n", i, pageTable->vpnMask);
         fprintf(stdout, "\nVP Lookup (%i): %X\n", i, pageTable->virtualPageLookup);
         fprintf(stdout, "\nVPN (%i): %X\n", i, pageTable->virtualPageNumber);
        fprintf(stdout, "Offset (%i): %X\n", i, pageTable->offset);
         std::cout << std::endl;
      }
   }

   pageTable->instructionsProcessed++; // ensures correct amount of addreesses are processed
}

uint32_t virtualAddressToPageNum(uint32_t virtualAddress, uint32_t mask, uint32_t shift) {
   uint32_t result;
   result = mask & virtualAddress;
   result = result >> shift;
   return result;
}