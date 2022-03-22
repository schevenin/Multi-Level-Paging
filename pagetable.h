/**
 * @file pagetable.h
 * @author Rogelio Schevenin, Sawyer Thompson
 * @redID 824107681, 823687079
 * @brief Header containing PageTable structure
 * @date 2022-03-21
 */

#ifndef PAGETABLE_H
#define PAGETABLE_H

#include "level.h"

#define DEFAULTADDRLIMIT -1 // default limit to number of addresses to process
#define DEFAULTSIZE 32      // default number of bits in address
#define DEFAULTTLBSIZE 0    // default TLB size
#define DEFAULTLRUSIZE 10   // default LRU size

/**
 * @brief PageTable structure
 * Stores PageTable attributes
 */
struct PageTable
{
  struct Level *rootLevelPtr; // pointer to level struct

  uint32_t numLevels;         // level count
  uint32_t offsetSize;        // gets offset of addresses
  uint32_t pageSize;          // gets pagesize
  uint32_t totalPageBits;     // sum of all pages bit sizes
  uint32_t addressCount;      // keeps track of instructions processed
  uint32_t totalBytes;        // total number of bytes across page table
  uint32_t pageTableHits;     // number of times a page was mapped
  uint32_t vpnMask;           // vpn mask
  uint32_t offsetMask;        // offset mask
  uint32_t *bitsPerLevel;     // the number of bits per level
  uint32_t *bitShiftPerLevel; // bitshifts per level
  uint32_t *entriesPerLevel;  // amount of entries per level
  uint32_t *pageLookupMask;   // page lookup masks
  uint32_t *pageLookup;       // page lookup
};

Map *pageLookup(PageTable *pageTable, uint32_t virtualAddress);                           // recursively performs a lookup of a VPN->PFN mapping in PageTable
void pageInsert(PageTable *pageTable, uint32_t virtualAddress, uint32_t newFrame);        // recursively performs an insertion of a VPN->VPN mapping in PageTable
void pageInsert(PageTable *level, uint32_t virtualAddress, uint32_t newFrame);            // recursively performs an insertion of a VPN->VPN mapping in PageTable
uint32_t virtualAddressToPageNum(uint32_t virtualAddress, uint32_t mask, uint32_t shift); // applies masking and shifting of a given address
uint32_t countPageTableSize(PageTable *pageTable, Level *level);                          // recursively counts the size in bytes of a PageTable

#endif