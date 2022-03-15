#include "pagetable.h"
#include "tracereader.h"
#include "cache.h"
#include "output_mode_helpers.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <unistd.h>

/* NEED TO INITIALIZE ALL VARIABLES IN PAGETABLE WITH GIVEN VALUES/DEFAULTS*/

int main(int argc, char **argv)
{
    int opt;                              // necessary optional argument
    FILE *tracefile;                      // tracefile
    FILE *addressOutput;                  // what we need to process
    addressOutput = fopen("addressOutput", "w");
    char *tracefileName;                  // tracefilename
    char *outputType = DEFAULTOUTPUTTYPE; // initialize outputType
    unsigned long i = 0;                  /* instructions processed */
    p2AddrTr trace;
    Map temp; //creates temp map to update map array in level.h
    uint32_t frame;
    // initializes a page table
    PageTable *pagetable = new PageTable();
    pagetable->offset = DEFAULTOFFSET;
    pagetable->numberofAddresses = NONUMBEROFARGUMENTS;

    // initialize OutputOptionsType
    OutputOptionsType *output = new OutputOptionsType();

    // runs through the optional arguments
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

    // need at least 2 arguements
    if (argc - optind < 2)
    {
        fprintf(stderr, "%s: too few arguments\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    tracefileName = argv[optind++]; // gets tracefile name

    // gets mandatory arguments for levels
    for (int i = optind; i < argc; i++)
    { 
        pagetable->numLevels++;                      // sets number of levels
        pagetable->numbits.push_back(atoi(argv[i])); // grabs amount of bits for each level
        pagetable->offset -= (atoi(argv[i]));        // gets offset by subtracting each page size
    }

    pagetable->LevelPtr = new Level[pagetable->numLevels];

    //checks if the tracefile is real
    if ((tracefile = fopen(tracefileName, "rb")) == NULL)
    {
        fprintf(stderr, "cannot open %s for reading\n", argv[1]);
        exit(1);
    }

    while (!feof(tracefile) && i != pagetable->numberofAddresses)
    {
        /* get next address and process */
        if (NextAddress(tracefile, &trace))
        {
            temp.frame = frame;
            frame++;
            
            pagetable->LevelPtr->map.push_back(temp);
            report_virtual2physical(trace.addr, frame);
            i++; // ensures correct amount of addreesses are processed
        }
    }
    fclose(tracefile);
    pagetable->pageSize = pow(2, pagetable->offset);    // get size = 2^offset
    report_summary(pagetable->pageSize, 0, 0, i, 0, 0); // creates summary, need to update 0's to actual arguments
    return 0;
};