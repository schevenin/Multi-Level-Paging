#include "pagetable.h"
#include "cache.h"

#include <math.h>
#include <fstream>
#include <unistd.h>

/* NEED TO INITIALIZE ALL VARIABLES IN PAGETABLE WITH GIVEN VALUES/DEFAULTS*/

void ProcessArguments(int argc, char **argv, PageTable *pageTable) {
    
    // default settings
    pageTable->offsetSize = DEFAULTOFFSET;
    pageTable->numberofAddresses = NONUMBEROFARGUMENTS;

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
            pageTable->numberofAddresses = atoi(optarg);
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

    
    // check mandatory arguments
    for (int i = optind; i < argc; i++)
    { 
        pageTable->numLevels++;                      // sets number of levels
        pageTable->numBits.push_back(atoi(argv[i])); // grabs amount of bits for each level
        pageTable->totalPageBits += (atoi(argv[i]));
        pageTable->offsetSize -= (atoi(argv[i]));        // gets offset by subtracting each page size
    }

    // verify working tracefile
    pageTable->tracefile = fopen(pageTable->tracefileName, "rb");
    if (pageTable->tracefile == NULL)
    {
        fprintf(stderr, "cannot open %s for reading\n", pageTable->tracefileName);
        exit(1);
    }

    // finally, assign pagetable variables
    pageTable->pageSize = pow(2, pageTable->offsetSize);    // get size = 2^offset
    pageTable->bitmask = new int[pageTable->numLevels]; //initialize bitmask array
    pageTable->LevelPtr = new Level[pageTable->numLevels]; //initialize level array
};

int main(int argc, char **argv)
{    

    // initialize OutputOptionsType
    OutputOptionsType *output = new OutputOptionsType();

    // handle and process arguments
    PageTable *pageTable = new PageTable();
    ProcessArguments(argc, argv, pageTable);
    pageTable->trace = new p2AddrTr();

    // insert into pages
    while (!feof(pageTable->tracefile) && pageTable->instructionsProcessed != pageTable->numberofAddresses)
    {
        pageInsert(pageTable, pageTable->trace->addr, pageTable->frame);
        std::cout << "Page Insert: " << std::hex << pageTable->trace->addr << " -> " << std::hex << pageTable->frame << std::endl;
        std::cout << "=================================" << std::endl;
    }



    //report_summary(pagetable->pageSize, 0, 0, pagetable->instructionsProcessed, 0, 0); // creates summary, need to update 0's to actual arguments
    return 0;
};

