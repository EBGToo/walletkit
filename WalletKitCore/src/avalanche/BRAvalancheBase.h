//
//  BRAvalancheBase.h
//  WalletKitCore
//
//  Created by Amit Shah on 2021-08-04.
//  Copyright © 2021 Breadwinner AG. All rights reserved.
//
//  See the LICENSE file at the project root for license information.
//  See the CONTRIBUTORS file at the project root for a list of contributors.
//
#ifndef BRAvalancheBase_h
#define BRAvalancheBase_h

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>
#include <memory.h>
#include <assert.h>

#include "support/BRBase.h"
#include "support/BRInt.h"

#if !defined(ASSERT_UNIMPLEMENTED)
#define ASSERT_UNIMPLEMENTED    assert(false);
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    AVALANCHE_CHAIN_TYPE_X,
    AVALANCHE_CHAIN_TYPE_C,
    AVALANCHE_CHAIN_TYPE_P,
} BRAvalancheChainType;

#define NUMBER_OF_AVALANCHE_CHAIN_TYPES    (1 + AVALANCHE_CHAIN_TYPE_P)

// The integer amount for Avalanche
typedef uint64_t BRAvalancheAmount;

#if 0
#define AVALANCHE_AMOUNT_DIGITS          (8)
#define AVALANCHE_AMOUNT_SCALE_FACTOR    (100000000)  // 1
#endif

// MARK: - Avalanche Hash

#define AVALANCHE_HASH_BYTES 32

typedef struct {
    uint8_t bytes[AVALANCHE_HASH_BYTES];            // SHA256, generally
} BRAvalancheHash;

#define AVALANCHE_HASH_EMPTY  ((BRAvalancheHash) { \
    0, 0, 0, 0,     0, 0, 0, 0, \
    0, 0, 0, 0,     0, 0, 0, 0, \
    0, 0, 0, 0,     0, 0, 0, 0, \
    0, 0, 0, 0,     0, 0, 0, 0, \
})

extern bool
avalancheHashIsEqual (const BRAvalancheHash h1,
                      const BRAvalancheHash h2);

extern bool
avalancheHashIsEmpty (BRAvalancheHash hash);

extern BRAvalancheHash
avalancheHashFromString(const char *input);

extern char *
avalancheHashToString (BRAvalancheHash hash);

// For use with BRSet
static inline uint32_t
avalancheHashSetValue (const BRAvalancheHash *hash) {
    // First foun bytes as a uint32; unlikely to be sufficient (?)
    return (uint32_t) ((UInt256 *) hash->bytes)->u32[0];
}


// Support

extern bool
avax_bech32_encode(char *const output, size_t *const out_len,
                   const char *const hrp, const size_t hrp_len,
                   const uint8_t *const data, const size_t data_len);

extern bool
avax_base32_encode(uint8_t *const out, size_t *out_len,
                   const uint8_t *const in, const size_t in_len);

extern int
avax_bech32_decode(char *hrp, uint8_t *data, size_t *data_len, const char *input);

extern int
avax_addr_bech32_decode(uint8_t *addr_data, size_t *addr_len, const char *hrp, const char *addr_str);

#ifdef __cplusplus
}
#endif

#endif /* BRAvalancheBase_h */