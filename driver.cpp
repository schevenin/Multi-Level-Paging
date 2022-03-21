/**
 * @file driver.cpp
 * @author Rogelio Schevenin, Sawyer Thompson
 * @redID 824107681, 823687079
 * @brief Multi Level Paging with TLB Cache entry point
 * @date 2022-03-21
 */

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string.h>
#include <math.h>
#include <map>
#include <ctype.h>

#include "tracereader.h"
#include "output_mode_helpers.h"
#include "pagetable.h"

/**
 * @brief Main Execution of Multi Level Paging with TLB Cache
 *
 * @param argc integer amount of arguments
 * @param argv character array of arguments
 * @return int value indicating exit status
 */
int main(int argc, char **argv)
{
    PageTable *pageTable;                      // instantiate page table
    OutputOptionsType *output;                 // instantiate output object
    std::map<uint32_t, uint32_t> TLB;          // instantiate map of <VPN, PFN>
    std::map<uint32_t, uint32_t> LRU;          // instantiate map of <VPN, Access Time>
    FILE *tracefile;                           // instantiate tracefile

    uint32_t physicalAddress;                  // instantiate physical address
    uint32_t VPN;                              // instantiate virtual page number
    uint32_t offset;                           // instantiate offset
    uint32_t PFN;                              // instantiate page frame number
    uint32_t newFrame;                         // instantiate new frame

    int pageSize;                              // instantiate page size
    int addressProcessingLimit;                // instantiate address limit
    int tlbCapacity;                           // instantiate size of TLB
    int lruCapacity;                           // instantiate size of LRU
    int cacheHits;                             // instantiate cache hits

    bool tlbHit;                               // instantiate TLB is hit
    bool ptHit;                                // instantiate page table is hit

    const char *outputType;                    // instantiate type of output

    pageTable = new PageTable();               // initialize page table
    output = new OutputOptionsType();          // initialize output object
    pageTable->offsetSize = DEFAULTSIZE;       // initialize offset size
    addressProcessingLimit = DEFAULTADDRLIMIT; // initialize address limit
    tlbCapacity = DEFAULTTLBSIZE;              // initialize size of TLB
    lruCapacity = DEFAULTLRUSIZE;              // initialize size of LRU
    outputType = DEFAULTOUTPUTTYPE;            // initialize output type

    // check optional arguments
    int opt;
    while ((opt = getopt(argc, argv, "n:c:o:")) != -1)
    {
        switch (opt)
        {
        // sets number of addresses needed to go through
        case 'n':
            addressProcessingLimit = atoi(optarg);

            // verify address limit is a number
            for (int i = 0; optarg[i] != 0; i++)
            {
                if (!isdigit(optarg[i]))
                {
                    std::cout << "Incorrect address processing limit--should be a number." << std::endl;
                    exit(EXIT_FAILURE);
                }
            }

            break;
        // gets cache capacity of adresses
        case 'c':
            tlbCapacity = atoi(optarg);

            // verify cache input is a number and verify valid cache capacity
            for (int i = 0; optarg[i] != 0; i++)
            {
                if ((!isdigit(optarg[i])) || (tlbCapacity < 0))
                {
                    std::cout << "Cache capacity must be a number, greater than or equal to 0" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
            
            break;
        // gets type of output
        case 'o':
            outputType = optarg;
            break;
        default:
            exit(EXIT_FAILURE);
        }
    }

    // require at least 2 arguments
    if (argc - optind < 2)
    {
        std::cout << "Level 0 page table must be at least 1 bit" << std::endl;
        exit(EXIT_FAILURE);
    }

    // verify working tracefile
    tracefile = fopen(argv[optind++], "rb");
    if (tracefile == NULL)
    {
        std::cout << "Unable to open <<" << argv[optind - 1] << ">>" << std::endl;
        exit(EXIT_FAILURE);
    }

    pageTable->numLevels = argc - optind;                        // get number of levels in page table
    pageTable->bitsPerLevel = new int[pageTable->numLevels];     // create array to store bit count per level
    pageTable->bitShiftPerLevel = new int[pageTable->numLevels]; // create array to store bit shift per level
    pageTable->entriesPerLevel = new int[pageTable->numLevels];  // create array to store entry counts per level

    // loop through each page level argument
    for (int i = optind; i < argc; i++)
    {
        // verify valid page size
        if (atoi(argv[i]) <= 0)
        {
            std::cout << "Level " << (i - optind) << " page table must be at least 1 bit" << std::endl;
            exit(EXIT_FAILURE);
        }

        pageTable->bitsPerLevel[i - optind] = atoi(argv[i]);                                  // store bits count for level i
        pageTable->entriesPerLevel[i - optind] = pow(2, pageTable->bitsPerLevel[i - optind]); // store entries for level i
        pageTable->totalPageBits += atoi(argv[i]);                                            // store total bits

        // verify valid bit sizes
        if ((pageTable->totalPageBits) > 28)
        {
            std::cout << "Too many bits used in page tables" << std::endl;
            exit(EXIT_FAILURE);
        }

        pageTable->offsetSize -= (atoi(argv[i]));                                           // offset at level i
        pageTable->bitShiftPerLevel[i - optind] = (DEFAULTSIZE - pageTable->totalPageBits); // amount of bit shift at level i
    }

    pageSize = pow(2, pageTable->offsetSize);                                            // set page size
    pageTable->vpnMask = ((1 << pageTable->totalPageBits) - 1) << pageTable->offsetSize; // set vpn mask
    pageTable->offsetMask = (1 << pageTable->offsetSize) - 1;                            // set offset mask
    pageTable->pageLookupMask = new uint32_t[pageTable->numLevels];                      // array of page lookup masks
    pageTable->pageLookup = new uint32_t[pageTable->numLevels];                          // array of page lookup masks

    // find page lookup masks at each level
    for (int i = 0; i < pageTable->numLevels; i++)
    {
        pageTable->pageLookupMask[i] = ((1 << pageTable->bitsPerLevel[i]) - 1) << (pageTable->bitShiftPerLevel[i]);
    }

    pageTable->addressCount = 0; // keep track of addresses processed from tracefile
    newFrame = 0;                // keep track of new frames mapped
    cacheHits = 0;               // keep track of amount of cache hits

    // process each address in tracefile until address limit is reached
    while (!feof(tracefile) && pageTable->addressCount != addressProcessingLimit)
    {
        // next address
        p2AddrTr *address_trace = new p2AddrTr(); // instantiate address trace

        // if another address exists
        if (NextAddress(tracefile, address_trace))
        {
            pageTable->addressCount++;                                                                     // increment addresses processed
            VPN = virtualAddressToPageNum(address_trace->addr, pageTable->vpnMask, pageTable->offsetSize); // find address VPN
            offset = virtualAddressToPageNum(address_trace->addr, pageTable->offsetMask, 0);               // find address offset

            // set page lookups (indexes) per level
            for (int i = 0; i < pageTable->numLevels; i++)
            {
                pageTable->pageLookup[i] = virtualAddressToPageNum(address_trace->addr, pageTable->pageLookupMask[i], pageTable->bitShiftPerLevel[i]);
            }

            // found VPN in TLB
            if (TLB.find(VPN) != TLB.end() && tlbCapacity > 0)
            {
                // TLB hit
                tlbHit = true;
                ptHit = false;
                cacheHits += 1;

                // PFN from TLB
                PFN = TLB[VPN];

                // TLB is full and VPN is not in LRU
                if ((TLB.size() == tlbCapacity) && (LRU[VPN] == NULL))
                {
                    uint32_t oldestKey;                    // VPN
                    uint32_t oldestValue;                  // access time
                    oldestValue = pageTable->addressCount; // start with current access time

                    // find oldest VPN in LRU
                    for (std::map<uint32_t, uint32_t>::iterator iter = LRU.begin(); iter != LRU.end(); ++iter)
                    {
                        // VPN access time is older than current access time and not VPN to be inserted
                        if ((iter->second < oldestValue) && (iter->second != NULL))
                        {
                            oldestKey = iter->first;    // update oldest VPN
                            oldestValue = iter->second; // update oldest access time
                        }
                    }

                    // erase oldest VPN in LRU
                    LRU.erase(oldestKey);
                }

                // Update VPN access time in LRU
                LRU[VPN] = pageTable->addressCount;
            }

            // not found in TLB
            else
            {
                // TLB miss, walk PageTable

                // search PageTable for VPN
                Map *found = pageLookup(pageTable, address_trace->addr);

                // VPN exists in PageTable
                if (found != NULL)
                {
                    // TLB miss, PageTable hit
                    tlbHit = false;
                    ptHit = true;
                    pageTable->pageTableHits += 1;

                    // update associated PFN
                    PFN = found->frame;
                }

                // VPN doesn't exist in PageTable
                else
                {
                    // TLB miss, PageTable miss
                    tlbHit = false;
                    ptHit = false;

                    // insert vpn and new frame into page table
                    pageInsert(pageTable, address_trace->addr, newFrame);

                    // update associated PFN
                    PFN = newFrame;
                }

                // if using cache
                if (tlbCapacity > 0)
                {
                    uint32_t oldestKey;   // oldest VPN
                    uint32_t oldestValue; // oldest access time

                    // if TLB is full
                    if (TLB.size() == tlbCapacity)
                    {
                        // TLB size > LRU size
                        if (tlbCapacity > lruCapacity)
                        {
                            // check all LRU items for oldest value
                            oldestValue = pageTable->addressCount; // start with current access time
                            for (std::map<uint32_t, uint32_t>::iterator iter = LRU.begin(); iter != LRU.end(); ++iter)
                            {
                                // if next access time in LRU is older
                                if (iter->second < oldestValue)
                                {
                                    oldestValue = iter->second;
                                    oldestKey = iter->first;
                                }
                            }
                        }

                        // TLB size < LRU size
                        else
                        {
                            // check all TLB items for oldest value
                            oldestValue = pageTable->addressCount; // start with current access time
                            for (std::map<uint32_t, uint32_t>::iterator iter = TLB.begin(); iter != TLB.end(); ++iter)
                            {
                                // if next access time in TLB is older
                                if (LRU[iter->first] < oldestValue)
                                {
                                    oldestKey = iter->first;
                                    oldestValue = LRU[iter->first];
                                }
                            }
                        }

                        // erase oldest VPN from TLB
                        TLB.erase(oldestKey);
                    }

                    // LRU is full and VPN is not in LRU
                    if ((LRU.size() == lruCapacity) && (LRU[VPN] == NULL))
                    {
                        // find oldest VPN in LRU
                        oldestValue = pageTable->addressCount; // start with current access time
                        for (std::map<uint32_t, uint32_t>::iterator iter = LRU.begin(); iter != LRU.end(); ++iter)
                        {
                            // VPN access time is older than current access time and not VPN to be inserted
                            if ((iter->second < oldestValue) && (iter->second != NULL))
                            {
                                oldestKey = iter->first;    // update oldest VPN
                                oldestValue = iter->second; // update oldest access time
                            }
                        }

                        // erase oldest VPN from LRU
                        LRU.erase(oldestKey);
                    }

                    // if VPN found in PageTable
                    if (ptHit)
                    {
                        // insert found VPN->PFN mapping into TLB and LRU
                        TLB[VPN] = found->frame;
                        LRU[VPN] = pageTable->addressCount;
                    }

                    // if VPN was not found in PageTable
                    else
                    {
                        // insert new VPN->PFN mapping into TLB and LRU
                        TLB[VPN] = newFrame;
                        LRU[VPN] = pageTable->addressCount;
                    }
                }

                // update new frame counter if PageTable was not hit
                if (!ptHit)
                {
                    newFrame++;
                }
            }

            // calculate physical address
            physicalAddress = (PFN * pageSize) + offset;

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
                fprintf(stdout, "%08X\n", offset);
            }
            if (strcmp(outputType, "v2p_tlb_pt") == 0)
            {
                report_v2pUsingTLB_PTwalk(address_trace->addr, physicalAddress, tlbHit, ptHit);
            }
        }
    }

    // output that doesn't need to loop
    if (strcmp(outputType, "summary") == 0)
    {
        report_summary(pageSize, cacheHits, pageTable->pageTableHits, pageTable->addressCount, newFrame, countPageTableSize(pageTable, pageTable->rootLevelPtr));
    }
    if (strcmp(outputType, "bitmasks") == 0)
    {
        report_bitmasks(pageTable->numLevels, pageTable->pageLookupMask);
    }

    return 0;
};
