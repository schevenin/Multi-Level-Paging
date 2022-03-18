#ifndef PAGETABLE_H
#define PAGETABLE_H

#include <stdio.h>
#include <stdint.h>
#include <vector>
#include <iostream>

#include "level.h"
#include "tracereader.h"
#include "output_mode_helpers.h"

#define DEFAULTADDRLIMIT -1
#define DEFAULTOFFSET 32
#define DEFAULTSIZE 32
#define DEFAULTCACHESIZE 0

struct PageTable
{
  struct Level *rootLevelPtr; // pointer to level struct

  int *EntryCount; // Size of each level

  int offsetSize;    // gets offset of addresses
  int pageSize;      // gets pagesize
  int numLevels;     /* number of levels*/
  int *bitsPerLevel; // creates an array that holds the number of bits for each level
  int *bitShift;     // creates an array of bitshifts for each level

  // masks
  uint32_t offsetMask;
  uint32_t vpnMask;
  uint32_t *pageLookupMask;

  int virtualPageLookup; // I think should be in level ( Each level will have 1)
  int virtualPageNumber; // total of virtual page lookup

  int totalPageBits;
  uint32_t offset;

  uint32_t frame;
  unsigned long instructionsProcessed = 0; // instructions
  p2AddrTr *trace;
};

Map *pageLookup(PageTable *pageTable, unsigned int virtualAddress);
void pageInsert(PageTable *pageTable, unsigned int *virtualAddress, uint32_t frame);

uint32_t virtualAddressToPageNum(uint32_t virtualAddress, uint32_t mask, uint32_t shift);

#endif