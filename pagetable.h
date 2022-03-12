#ifndef PAGETABLE_H
#define PAGETABLE_H

#include <stdio.h>
#include <stdint.h>
#include <vector>
#include "pagetable.h"
#define NONUMBEROFARGUMENTS -1
#define DEFAULTOFFSET 32
struct PageTable{
  struct Level *LevelPtr;
  int numLevels;  /* number of levels*/
  int* bitmask; //creates an array of bitmasks
  int* bitshift; //creates an array of bitshifts for each level
  std::vector<int> numbits; //creates an vetor that holds the number of bits for each level
  int numberofAddresses; //numberofAddresses that need to be processed
  int offset; //gets offset of addresses
  int pageSize; //gets pagesize
};


void pageInsert(PageTable page, uint32_t address, uint32_t frame); //inserts the page into the pagetable
#endif