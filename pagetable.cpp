#include "pagetable.h"

void pageInsert(PageTable *pageTable, uint32_t *address, uint32_t frame)
{
   /* get next address and process */
   if (NextAddress(pageTable->tracefile, pageTable->trace))
   {  
      pageTable->virtualPageNumber = virtualAddressToPageNum(*address, pageTable->vpnMask, pageTable->offsetSize) << pageTable->offsetSize;
      pageTable->offset = virtualAddressToPageNum(*address, pageTable->offsetMask, pageTable->offsetSize) << pageTable->offsetSize;

      std::cout << "=================================" << std::endl;
      fprintf(stdout, "Virtual Address: %08X\n", pageTable->trace->addr);
      fprintf(stdout, "VPN Mask: %X\n", pageTable->vpnMask);
      fprintf(stdout, "Offset mask: %08X\n", pageTable->offsetMask);
      fprintf(stdout, "VPN: %08X\n", pageTable->virtualPageNumber);
      fprintf(stdout, "Offset: %08X\n", pageTable->offset);

      for (int i = 0; i < pageTable->numLevels; i++)
      {
         pageTable->virtualPageLookup = virtualAddressToPageNum(*address, pageTable->bitmask[i], pageTable->bitshift[i]) << pageTable->bitshift[i];
         fprintf(stdout, "\nLevel: %i\n", i);
         fprintf(stdout, "Virtual page lookup mask (%i): %08X\n", i, pageTable->bitmask[i]);
         fprintf(stdout, "Virtual page lookup (%i): %08X\n", i, pageTable->virtualPageLookup);
      }

   }

   pageTable->instructionsProcessed++; // ensures correct amount of addreesses are processed
}

uint32_t virtualAddressToPageNum(uint32_t virtualAddress, uint32_t mask, uint32_t shift)
{
   uint32_t result;
   result = mask & virtualAddress;
   result = result >> shift;
   return result;
}