#include "pagetable.h"
#include "cache.h"
#include "output_mode_helpers.h"

#include <math.h>
#include <fstream>
#include <unistd.h>

/* NEED TO INITIALIZE ALL VARIABLES IN PAGETABLE WITH GIVEN VALUES/DEFAULTS*/

void ProcessArguments(int argc, char **argv, PageTable *pagetable) {
    
    // default settings
    pagetable->offset = DEFAULTOFFSET;
    pagetable->numberofAddresses = NONUMBEROFARGUMENTS;

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
            pagetable->numberofAddresses = atoi(optarg);
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
    pagetable->tracefileName = argv[optind++]; // gets tracefile name

    
    // check mandatory arguments
    for (int i = optind; i < argc; i++)
    { 
        pagetable->numLevels++;                      // sets number of levels
        pagetable->numbits.push_back(atoi(argv[i])); // grabs amount of bits for each level
        pagetable->offset -= (atoi(argv[i]));        // gets offset by subtracting each page size
    }

    // verify working tracefile
    pagetable->tracefile = fopen(pagetable->tracefileName, "rb");
    if (pagetable->tracefile == NULL)
    {
        fprintf(stderr, "cannot open %s for reading\n", pagetable->tracefileName);
        exit(1);
    }

    // finally, assign pagetable variables
    pagetable->pageSize = pow(2, pagetable->offset);    // get size = 2^offset
    pagetable->bitmask = new int[pagetable->numLevels]; //initialize bitmask array
    pagetable->LevelPtr = new Level[pagetable->numLevels]; //initialize level array
};

int main(int argc, char **argv)
{    

    // initialize OutputOptionsType
    OutputOptionsType *output = new OutputOptionsType();

    // handle and process arguments
    PageTable *pagetable = new PageTable();
    ProcessArguments(argc, argv, pagetable);
    pagetable->trace = new p2AddrTr();
    pageInsert(pagetable, pagetable->trace->addr, pagetable->frame);




   // report_summary(pagetable->pageSize, 0, 0, i, 0, 0); // creates summary, need to update 0's to actual arguments
    return 0;
};

