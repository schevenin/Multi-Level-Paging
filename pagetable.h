#ifndef PAGETABLE_H
#define PAGETABLE_H

#include <stdio.h>
#include <stdint.h>
#include <vector>
#include <iostream>

#include "level.h"
#include "tracereader.h"
#include "output_mode_helpers.h"

#define NONUMBEROFARGUMENTS -1
#define DEFAULTOFFSET 32
#define DEFAULTSIZE 32

struct PageTable
{
  struct Level *rootLevelPtr;   // pointer to level struct
  uint32_t *bitmask;             // creates an array of bitmasks
  int *bitshift;            // creates an array of bitshifts for each level
  int *EntryCount; //Size of each level
  int numLevels;            /* number of levels*/
  
  int *numBits; // creates an array that holds the number of bits for each level
  int offsetSize;          // gets offset of addresses
  int pageSize;          // gets pagesize

  uint32_t offsetMask;
  uint32_t pageMask;
  uint32_t vpnMask; 

  int virtualPageLookup; //I think should be in level ( Each level will have 1)
  int virtualPageNumber; //total of virtual page lookup
  
  int totalPageBits;
  uint32_t offset;
  
  uint32_t frame;
  FILE *tracefile;
  char *tracefileName;
  unsigned long instructionsProcessed = 0; // instructions
  p2AddrTr *trace;
};

Map pageLookup(PageTable *pageTable, unsigned int vritualAddress);
void pageInsert(PageTable *pageTable, uint32_t *address, uint32_t frame);

uint32_t virtualAddressToPageNum(uint32_t virtualAddress, uint32_t mask, uint32_t shift);

#endif