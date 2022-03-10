#include <stdio.h>
#include <stdint.h>
#include <vector>
#define NONUMBEROFARGUMENTS -1
struct PageTable{

  int numLevels;  /* number of levels*/
  int* bitmask; //creates an array of bitmasks
  int* bitshift; //creates an array of bitshifts for each level
  std::vector<int> numbits; //creates an vetor that holds the number of bits for each level
  int numberofAddresses; //numberofAddresses that need to be processed
};


void pageInsert(PageTable page, uint32_t address, uint32_t frame); //inserts the page into the pagetable