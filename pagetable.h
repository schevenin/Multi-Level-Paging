#include <stdio.h>
#include <stdint.h>
struct PageTable{

  int levelcount;  /* display count of levels */
  int* bitmask; //creates an array of bitmasks
  int* bitshift; //creates an array of bitshifts for each level
  int* entrycount; //creates an array of entrycount for each level
  
};

void pageInsert(PageTable page, uint32_t address, uint32_t frame); //inserts the page into the pagetable