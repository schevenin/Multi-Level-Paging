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

  int numLevels;     // level count
  int offsetSize;    // gets offset of addresses
  int pageSize;      // gets pagesize
  int totalPageBits; // sum of all pages bit sizes
  int addressCount;  // keeps track of instructions processed
  int totalBytes;    // total number of bytes across page table
  int pageTableHits; // number of times a page was mapped

  int *bitsPerLevel;     // the number of bits per level
  int *bitShiftPerLevel; // bitshifts per level
  int *entriesPerLevel;  // amount of entries per level

  uint32_t vpnMask;         // vpn mask
  uint32_t offsetMask;      // offset mask
  uint32_t *pageLookupMask; // page lookup masks
  uint32_t *pageLookup;     // page lookup
};

Map *pageLookup(PageTable *pageTable, uint32_t virtualAddress);
void pageInsert(PageTable *pageTable, uint32_t virtualAddress, uint32_t newFrame);
void pageInsert(PageTable *level, uint32_t virtualAddress, uint32_t newFrame);

uint32_t virtualAddressToPageNum(uint32_t virtualAddress, uint32_t mask, uint32_t shift);
int countPageTableSize(PageTable *pageTable, Level *level);

#endif