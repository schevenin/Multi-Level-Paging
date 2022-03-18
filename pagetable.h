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

  int numLevels;   /* number of levels*/
  int *EntryCount; // Size of each level
  int offsetSize;  // gets offset of addresses
  int pageSize;    // gets pagesize
  int totalPageBits;
  int *bitsPerLevel;         // creates an array that holds the number of bits for each level
  int *bitShift;             // creates an array of bitshifts for each level
  int instructionsProcessed; // keeps track of instructions processed

  // masks and addresses
  uint32_t offsetMask;      // offset mask
  uint32_t vpnMask;         // vpn mask
  uint32_t *pageLookupMask; // page lookup masks

  uint32_t virtualPageNumber; // vpn
  uint32_t offset;            // offset
  uint32_t virtualPageLookup; // page lookup

  // helper
  p2AddrTr *trace;
  uint32_t frame;
};

Map *pageLookup(PageTable *pageTable, uint32_t virtualAddress);
void pageInsert(PageTable *pageTable, uint32_t virtualAddress, uint32_t frame);

uint32_t virtualAddressToPageNum(uint32_t virtualAddress, uint32_t mask, uint32_t shift);

#endif