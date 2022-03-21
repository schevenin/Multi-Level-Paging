
/**
 * @file map.h
 * @author Rogelio Schevenin, Sawyer Thompson
 * @redID 824107681, 823687079
 * @brief Header containing VPN->PFN Map structure
 * @date 2022-03-21
 */

#ifndef MAP_H
#define MAP_H

#include <unistd.h>
#include <stdint.h>

/**
 * @brief Map structure
 * Stores a single VPN->PFN mapping
 */
struct Map
{
    uint32_t frame; // physical frame
    uint32_t vpn;   // virtual page number
};

#endif