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

struct PageTable
{
  struct Level *LevelPtr;   // pointer to level struct
  int numLevels;            /* number of levels*/
  int *bitmask;             // creates an array of bitmasks
  int *bitshift;            // creates an array of bitshifts for each level
  std::vector<int> numbits; // creates an vetor that holds the number of bits for each level
  int numberofAddresses;


  Map temp; //creates temp map to update map array in level.h
  uint32_t frame;
  int virtualPageNumber;
  int totalPageBits;
  int offset;
  int offsetSize;            // gets offset of addresses
  int pageSize;          // gets pagesize
  FILE *tracefile;
  char *tracefileName;
  unsigned long instructionsProcessed = 0; // instructions
  p2AddrTr *trace;
};

Map pageLookup(PageTable *pageTable, unsigned int vritualAddress);
void pageInsert(PageTable *pageTable, uint32_t address, uint32_t frame);

void createBitmaskPageNumber(PageTable *pageTable); // creates bitmask page number
void createBitmaskOffset(PageTable *pageTable);     // creates bitmask offset

#endif