// block.h
#ifndef BLOCK_H
#define BLOCK_H

#include <stdint.h>

struct block_header {
    uint32_t version;
    uint8_t  prev_hash[32];
    uint8_t  merkle_root[32];
    uint32_t timestamp;
    uint32_t bits;
    uint32_t nonce;
};

#endif

