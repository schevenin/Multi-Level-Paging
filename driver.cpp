#include "pagetable.h"
#include "output_mode_helpers.h"
#include "string.h"

#include <unordered_map>
#include <map>
#include <math.h>
#include <fstream>
#include <unistd.h>

int main(int argc, char **argv)
{
    PageTable *pageTable;      // instantiate page table
    OutputOptionsType *output; // instantiate output object

    std::map<uint32_t, uint32_t> TLB; // cache table of <VPN, PFN>
    std::map<uint32_t, uint32_t> LRU; // least recent accessed table of <VPN, Access Time>

    uint32_t pageSize;          // instantiate page size
    int addressProcessingLimit; // instantiate address limit
    int cacheCapacity;          // instantiate size of TLB
    char *outputType;           // instantiate type of output
    FILE *tracefile;            // instantiate tracefile
    uint32_t physicalAddress;   // instantiate physical address
    bool tlbhit;                // instantiate TLB is hit
    bool pthit;                 // instantiate page table is hit

    pageTable = new PageTable();               // initialize page table
    output = new OutputOptionsType();          // initialize output object
    pageTable->offsetSize = DEFAULTOFFSET;     // initialize offset size
    addressProcessingLimit = DEFAULTADDRLIMIT; // initialize address limit
    cacheCapacity = DEFAULTCACHESIZE;          // initialize size of TLB
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
            cacheCapacity = atoi(optarg);

            if (cacheCapacity < 0) {
                fprintf(stderr, "Cache capacity must be a number, greater than or equal to 0");
                exit(EXIT_FAILURE);
            }

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
        fprintf(stderr, "Level 0 page table must be at least 1 bit");
        exit(EXIT_FAILURE);
    }

    // verify working tracefile
    tracefile = fopen(argv[optind++], "rb");
    if (tracefile == NULL)
    {
        fprintf(stderr, "Unable to open <<%s>>", argv[optind++]);
        exit(EXIT_FAILURE);
    }

    pageTable->numLevels = argc - optind;                    // get number of levels in page table
    pageTable->bitsPerLevel = new int[pageTable->numLevels]; // create array to store bit count per level
    pageTable->bitShift = new int[pageTable->numLevels];     // create array to store bit shift per level
    pageTable->entriesPerLevel = new int[pageTable->numLevels];

    // loop through each page level
    for (int i = optind; i < argc; i++)
    {

        if (atoi(argv[i]) <= 0) {
            fprintf(stderr, "Level %i page table must be at least 1 bit", i-optind);
            exit(EXIT_FAILURE);
        }

        // counting bits at each level
        pageTable->bitsPerLevel[i - optind] = atoi(argv[i]);                                  // store bits count for level i
        pageTable->entriesPerLevel[i - optind] = pow(2, pageTable->bitsPerLevel[i - optind]); // store entries for level i

        // finding bitshift for each level
        pageTable->totalPageBits += atoi(argv[i]);                                  // total page bits at level i
        
        if ((pageTable->totalPageBits) > 28) {
            fprintf(stderr, "Too many bits used in page tables");
            exit(EXIT_FAILURE);
        }

        pageTable->offsetSize -= (atoi(argv[i]));                                   // offset at level i
        pageTable->bitShift[i - optind] = (DEFAULTSIZE - pageTable->totalPageBits); // amount of bit shift at level i
    }

    // create vpn, offset, masks
    pageSize = pow(2, pageTable->offsetSize);                                            // set page size
    pageTable->vpnMask = ((1 << pageTable->totalPageBits) - 1) << pageTable->offsetSize; // vpn mask
    pageTable->offsetMask = (1 << pageTable->offsetSize) - 1;                            // offset mask
    pageTable->pageLookupMask = new uint32_t[pageTable->numLevels];                      // array of page lookup masks
    pageTable->pageLookup = new uint32_t[pageTable->numLevels];                          // array of page lookup masks

    // find page lookup masks at each level
    for (int i = 0; i < pageTable->numLevels; i++)
    {
        pageTable->pageLookupMask[i] = ((1 << pageTable->bitsPerLevel[i]) - 1) << (pageTable->bitShift[i]);
    }

    pageTable->addressCount = 0;
    uint32_t newFrame = 0;

    // process each address within address to processing limits
    while (!feof(tracefile) && pageTable->addressCount != addressProcessingLimit)
    {
        // next address
        p2AddrTr *address_trace = new p2AddrTr();

        // if another address exists
        if (NextAddress(tracefile, address_trace))
        {
            pageTable->addressCount++;                                                                                // keeping track of
            pageTable->vpn = virtualAddressToPageNum(address_trace->addr, pageTable->vpnMask, pageTable->offsetSize); // find address VPN
            pageTable->offset = virtualAddressToPageNum(address_trace->addr, pageTable->offsetMask, 0);               // find address offset

            // page lookups per level
            for (int i = 0; i < pageTable->numLevels; i++)
            {
                pageTable->pageLookup[i] = virtualAddressToPageNum(address_trace->addr, pageTable->pageLookupMask[i], pageTable->bitShift[i]);
            }

            uint32_t PFN; // PFN that is mapped to VPN

            // found in TLB
            if (TLB.find(pageTable->vpn) != TLB.end() && cacheCapacity > 0)
            {
                // TLB hit                
                tlbhit = true;
                pthit = false;

                // PFN from TLB
                PFN = TLB[pageTable->vpn];

                // update LRU with most recent addressCount
                LRU[pageTable->vpn] = pageTable->addressCount;
            }

            // not found in TLB
            else
            {
                // TLB miss, walk PageTable

                // search PageTable for VPN
                Map *found = pageLookup(pageTable, pageTable->vpn);

                // found VPN in PageTable
                if (found != NULL)
                {
                    // TLB miss, PageTable hit
                    tlbhit = false;
                    pthit = true;

                    // check if cache is full
                    if (TLB.size() == cacheCapacity)
                    {
                        // find oldest VPN in LRU
                        uint32_t oldestKey;
                        uint32_t oldestValue;
                        oldestValue = pageTable->addressCount;
                        for (std::map<uint32_t, uint32_t>::iterator iter = LRU.begin(); iter != LRU.end(); ++iter)
                        {
                            if (oldestValue > iter->second)
                            {
                                oldestKey = iter->first;
                                oldestValue = iter->second;
                            }
                        }

                        // erase oldest from TLB and LRU
                        TLB.erase(oldestKey);
                        LRU.erase(oldestKey);
                    }

                    // insert into TLB and LRU
                    TLB[found->vpn] = found->frame;
                    LRU[found->vpn] = pageTable->addressCount;

                    PFN = found->frame;
                }

                // inserting VPN into PageTable
                else
                {
                    // TLB miss, PageTable miss
                    tlbhit = false;
                    pthit = false;

                    // insert vpn and new frame into page table
                    pageInsert(pageTable, pageTable->vpn, newFrame);

                    // check if cache is full
                    if (TLB.size() == cacheCapacity)
                    {
                        // find oldest VPN in LRU
                        uint32_t oldestKey;
                        uint32_t oldestValue;
                        oldestValue = pageTable->addressCount;
                        for (std::map<uint32_t, uint32_t>::iterator iter = LRU.begin(); iter != LRU.end(); ++iter)
                        {
                            if (oldestValue > iter->second)
                            {
                                oldestKey = iter->first;
                                oldestValue = iter->second;
                            }
                        }

                        // erase oldest from TLB and LRU
                        TLB.erase(oldestKey);
                        LRU.erase(oldestKey);
                    }

                    // insert into TLB and LRU
                    TLB[pageTable->vpn] = newFrame;
                    LRU[pageTable->vpn] = pageTable->addressCount;

                    PFN = newFrame;
                    newFrame++;
                }
            }

            physicalAddress = (PFN * pageSize) + pageTable->offset;

            // output that needs to loop
            if (strcmp(outputType, "virtual2physical") == 0)
            {
                report_virtual2physical(address_trace->addr, physicalAddress);
            }
            if (strcmp(outputType, "vpn2pfn") == 0)
            {
                report_pagemap(pageTable->numLevels, pageTable->pageLookup, PFN);
            }
            if (strcmp(outputType, "offset") == 0)
            {
                fprintf(stdout, "%08X\n", pageTable->offset);
            }
            if (strcmp(outputType, "v2p_tlb_pt") == 0)
            {
                report_v2pUsingTLB_PTwalk(address_trace->addr, physicalAddress, tlbhit, pthit);
            }
        }
    }

    // output that doesn't need to loop
    if (strcmp(outputType, "summary") == 0)
    {
        report_summary(pageSize, cacheCapacity, pageTable->pageTableHits, pageTable->addressCount, newFrame, countPageTableSize(pageTable, pageTable->rootLevelPtr));
    }

    if (strcmp(outputType, "bitmasks") == 0)
    {
        report_bitmasks(pageTable->numLevels, pageTable->pageLookupMask);
    }


    // report_summary(pagetable->pageSize, 0, 0, pagetable->instructionsProcessed, 0, 0); // creates summary, need to update 0's to actual arguments
    return 0;
};
