#include "pagetable.h"
#include "cache.h"

#include <math.h>
#include <fstream>
#include <unistd.h>

int main(int argc, char **argv)
{
    PageTable *pageTable;      // instantiate page table
    OutputOptionsType *output; // instantiate output object

    int pageSize;               // instantiate page size
    int addressProcessingLimit; // instantiate address limit
    int cacheCapacity;          // instantiate size of TLB
    char *outputType;           // instantiate type of output
    FILE *tracefile;            // instantiate tracefile

    pageTable = new PageTable();               // initialize page table
    output = new OutputOptionsType();          // initialize output object
    pageTable->offsetSize = DEFAULTOFFSET;     // initialize offset size
    addressProcessingLimit = DEFAULTADDRLIMIT; // initialize address limit
    cacheCapactiy = 0;                         // initialize size of TLB
    outputType = DEFAULTOUTPUTTYPE;            // initialize output type

    // check optional arguments
    int opt;
    while ((opt = getopt(argc, argv, "n:c:o:")) != -1)
    {
        switch (opt)
        {
        case 'n':
            // sets number of addresses needed to go through
            addressProcessingLimit = atoi(optarg);
            break;
        case 'c':
            // gets cache capacity of adresses
            cacheCapactiy = atoi(optarg);
            break;
        case 'o':
            // gets type of output
            outputType = optarg;
            break;
        default:
            exit(EXIT_FAILURE);
        }
    }

    // require at least 2 arguments
    if (argc - optind < 2)
    {
        fprintf(stderr, "%s: too few arguments\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // verify working tracefile
    tracefile = fopen(argv[optind++], "rb");
    if (tracefile == NULL)
    {
        fprintf(stderr, "cannot open %s for reading: \n", argv[optind++]);
        exit(1);
    }

    pageSize = pow(2, pageTable->offsetSize);                // set page size
    pageTable->numLevels = argc - optind;                    // get number of levels in page table
    pageTable->bitsPerLevel = new int[pageTable->numLevels]; // create array to store bit count per level
    pageTable->bitShift = new int[pageTable->numLevels];     // create array to store bit shift per level
    pageTable->entriesPerLevel = new int[pageTable->numLevels];

    // loop through each page level
    for (int i = optind; i < argc; i++)
    {
        // counting bits at each level
        pageTable->bitsPerLevel[i - optind] = atoi(argv[i]);                             // store bits count for level i
        pageTable->entriesPerLevel[i - optind] = pow(2, pageTable->bitsPerLevel[i - optind]); // store entries for level i

        // finding bitshift for each level
        pageTable->totalPageBits += atoi(argv[i]);                                  // total page bits at level i
        pageTable->offsetSize -= (atoi(argv[i]));                                   // offset at level i
        pageTable->bitShift[i - optind] = (DEFAULTSIZE - pageTable->totalPageBits); // amount of bit shift at level i
    }

    // create vpn, offset, masks
    pageTable->vpnMask = ((1 << pageTable->totalPageBits) - 1) << pageTable->offsetSize; // vpn mask
    pageTable->offsetMask = (1 << pageTable->offsetSize) - 1;                            // offset mask
    pageTable->pageLookupMask = new uint32_t[pageTable->numLevels];                      // array of page lookup masks
    pageTable->pageLookup = new uint32_t[pageTable->numLevels];                      // array of page lookup masks

    // find page lookup masks at each level
    for (int i = 0; i < pageTable->numLevels; i++)
    {
        pageTable->pageLookupMask[i] = ((1 << pageTable->bitsPerLevel[i]) - 1) << (pageTable->bitShift[i]);
    }

    // FUNCTIONALITY: lookup, insert, update

    pageTable->rootLevelPtr = new Level[pageTable->numLevels]; // root level pointer
    pageTable->instructionsProcessed = 0;

    // remain within address to process limits
    while (!feof(tracefile) && pageTable->instructionsProcessed != addressProcessingLimit)
    {

        // next address
        p2AddrTr *address_trace = new p2AddrTr();

        // if another address exists
        if (NextAddress(tracefile, address_trace))
        {

            // find address VPN
            pageTable->vpn = virtualAddressToPageNum(address_trace->addr, pageTable->vpnMask, pageTable->offsetSize) << pageTable->offsetSize;
            // find address offset
            pageTable->offset = virtualAddressToPageNum(address_trace->addr, pageTable->offsetMask, 0);



            // print details
            std::cout << "=================================" << std::endl;
            fprintf(stdout, "Virtual Address:       %08X\n", address_trace->addr);
            fprintf(stdout, "VPN Mask:              %08X\n", pageTable->vpnMask);
            fprintf(stdout, "VPN:                   %08X\n", pageTable->vpn);
            fprintf(stdout, "Offset mask:           %08X\n", pageTable->offsetMask);
            fprintf(stdout, "Offset:                %08X\n", pageTable->offset);
            fprintf(stdout, "\nVirtual Page Lookups:\n");            


            // page lookups per level
            for (int i = 0; i < pageTable->numLevels; i++)
            {
                pageTable->pageLookup[i] = virtualAddressToPageNum(address_trace->addr, pageTable->pageLookupMask[i], pageTable->bitShift[i]) << pageTable->bitShift[i];
                
                // print details
                fprintf(stdout, "Page Lookup Mask  (%i): %08X\n", i, pageTable->pageLookupMask[i]);
                fprintf(stdout, "Page Lookup Num   (%i): %08X\n", i, pageTable->pageLookup[i]);
                fprintf(stdout, "Page Lookup Index (%i): %i/%i\n", i, (pageTable->pageLookup[i]>>pageTable->bitShift[i]), pageTable->entriesPerLevel[i]);
            }
            
            pageTable->instructionsProcessed++;
        }

        // look for page in TLB
        // pageLookup(pageTable, &pageTable->trace->addr, pageTable->frame);
        // pageInsert(pageTable, &pageTable->trace->addr, pageTable->frame);
    }

    // report_summary(pagetable->pageSize, 0, 0, pagetable->instructionsProcessed, 0, 0); // creates summary, need to update 0's to actual arguments
    return 0;
};


