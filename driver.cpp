#include "pagetable.h"
#include "cache.h"

#include <math.h>
#include <fstream>
#include <unistd.h>

int main(int argc, char **argv)
{
    // initialize OutputOptionsType
    OutputOptionsType *output = new OutputOptionsType();

    // handle and process arguments
    PageTable *pageTable = new PageTable();

    // default settings
    pageTable->offsetSize = DEFAULTOFFSET;
    int numberOfAddresses = NONUMBEROFARGUMENTS;
    
    // for output
    char *outputType = DEFAULTOUTPUTTYPE;

    // check optional arguments
    int opt;
    while ((opt = getopt(argc, argv, "n:c:o:")) != -1)
    {
        switch (opt)
        {
        case 'n':
            // sets number of addresses needed to go through
            numberOfAddresses = atoi(optarg);
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

    // require 2 arguments
    if (argc - optind < 2)
    {
        fprintf(stderr, "%s: too few arguments\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    pageTable->tracefileName = argv[optind++]; // gets tracefile name

    pageTable->numLevels = argc - optind;
    pageTable->numBits = new int[pageTable->numLevels];
    pageTable->bitshift = new int[pageTable->numLevels];
    // check mandatory arguments
    int count = 0;
    for (int i = optind; i < argc; i++)
    {
        pageTable->numBits[count] = atoi(argv[i]); // grabs amount of bits for each level
         //std::cout <<pageTable->numBits[i]<< std::endl;
        pageTable->totalPageBits += atoi(argv[i]);
        pageTable->offsetSize -= (atoi(argv[i])); // gets offset by subtracting each page size
        pageTable->bitshift[count] = (DEFAULTSIZE - pageTable->totalPageBits);
        count++;
    }
    // verify working tracefile
    pageTable->tracefile = fopen(pageTable->tracefileName, "rb");
    if (pageTable->tracefile == NULL)
    {
        fprintf(stderr, "cannot open %s for reading\n", pageTable->tracefileName);
        exit(1);
    }

    // finally, assign pagetable variables
    pageTable->pageSize = pow(2, pageTable->offsetSize);     // get size = 2^offset
    pageTable->bitmask = new uint32_t[pageTable->numLevels]; // initialize bitmask array
    pageTable->rootLevelPtr = new Level[pageTable->numLevels];   // initialize level array

    // offset mask
    pageTable->offsetMask = (1 << pageTable->offsetSize) - 1;

    //PageLookUp mask
    for (int i = 0; i < pageTable->numLevels; i++)
    {
        pageTable->bitmask[i] = (1 << pageTable->numBits[i]) - 1;
        pageTable->bitmask[i] = pageTable->bitmask[i] << (pageTable->bitshift[i]);
        
        //std::cout <<pageTable->numBits[i]<< std::endl;
       // std::cout <<pageTable->bitmask[i]<< std::endl;
    }

    //VPN mask
    pageTable->vpnMask = (1 << pageTable->totalPageBits) - 1;

    pageTable->vpnMask = pageTable->vpnMask << pageTable->offsetSize;
    //std::cout << std::hex<<pageTable->vpnMask<< std::endl;
    pageTable->trace = new p2AddrTr();

    // insert into pages
    while (!feof(pageTable->tracefile) && pageTable->instructionsProcessed <= numberOfAddresses)
    {
        pageInsert(pageTable, &pageTable->trace->addr, pageTable->frame);
    }
    // report_summary(pagetable->pageSize, 0, 0, pagetable->instructionsProcessed, 0, 0); // creates summary, need to update 0's to actual arguments
    return 0;
};
