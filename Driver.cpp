#include "pagetable.h"
#include "tracereader.h"
#include "cache.h"
#include "output_mode_helpers.h"
#include <math.h> 
#include <iostream>
#include <unistd.h>

/* NEED TO INITIALIZE ALL VARIABLES IN PAGETABLE WITH GIVEN VALUES/DEFAULTS*/

int main(int argc, char **argv)
{
    int opt; // necessary optional argument
    char *tracefile;
    int *entrycount;
    char* outputType = DEFAULTOUTPUTTYPE;
    
    // initializes a page table
    PageTable *pagetable = new PageTable();
    pagetable->offset = DEFAULTOFFSET;
    //initialize OutputOptionsType
    OutputOptionsType *output= new OutputOptionsType();
    
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
            cacheCapactiy = atoi(optarg);
            break;
        case 'o':
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

    tracefile = argv[optind++]; // gets tracefile

    // gets mandatory arguments for levels
    for (int i = optind; i < argc; i++)
    {
        pagetable->numLevels++; //sets number of levels
        pagetable->numbits.push_back(atoi(argv[i])); // grabs amount of bits for each level
        pagetable->offset -= (atoi(argv[i])); //gets offset by subtracting each page size
    }

    pagetable->pageSize = pow(2,pagetable->offset);  //get size = 2^offset 
    std::cout << "Page Size" << pagetable->pageSize; //eventually update to summary in output_mode_helper
    return 0;
};

// STUFF BELOW IS FOR GRAABBING ADDRESSES OF TRACEFILES AND STUFF

// FILE *ifp;           /* trace file */
// unsigned long i = 0; /* instructions processed */
// p2AddrTr trace;      /* traced address */

// /* check usage */
// if (argc != 2)
// {
//     fprintf(stderr, "usage: %s input_byutr_file\n", argv[0]);
//     exit(1);
// }

// /* attempt to open trace file */
// if ((ifp = fopen(argv[1], "rb")) == NULL)
// {
//     fprintf(stderr, "cannot open %s for reading\n", argv[1]);
//     exit(1);
// }

// while (!feof(ifp))
// {
//     /* get next address and process */
//     if (NextAddress(ifp, &trace))
//     {
//         AddressDecoder(&trace, stdout);
//         i++;
//         // std::cout<< &trace;
//         if ((i % 100000) == 0)
//             fprintf(stderr, "%dK samples processed\r", i / 100000);
//     }
// }

// /* clean up and return success */
// fclose(ifp);
