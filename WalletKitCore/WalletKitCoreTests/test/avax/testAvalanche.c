//
//  testAvalanche.c
//  WalletKitCore
//
//  Created by Amit Shah on 2021-08-04.
//  Copyright © 2021 Breadwinner AG. All rights reserved.
//
//  See the LICENSE file at the project root for license information.
//  See the CONTRIBUTORS file at the project root for a list of contributors.
//
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

// Key WalletKit includes
#include "support/BRArray.h"
#include "support/BRInt.h"
#include "support/BRBIP39Mnemonic.h"

#include "avalanche/BRAvalanche.h"
#include "avalanche/BRAvalancheSupport.h"

// MARK: - Utils Test

static void
runAvalancheUtilsCB58Test (void) {
    static struct { char *data; char *cb58; } vectors[] = {
        { "Hello world", "32UWxgjUJd9s6Kyvxjj1u" },
        { NULL, NULL }
    };
    printf("TST:    Avalanche Utils CB58\n");

    for (size_t index = 0; vectors[index].data != NULL; index++ ) {
        char *data = vectors[index].data;
        char *cb58 = vectors[index].cb58;

        char *cb58Test = BRAvalancheCB58CheckEncodeCreate ((uint8_t*) data, strlen(data));
        assert (0 == strcmp (cb58Test, cb58));

        size_t   dataTestLength;
        uint8_t *dataTest = BRAvalancheCB58CheckDecodeCreate (cb58Test, &dataTestLength);
        assert (dataTestLength == strlen (data));
        assert (0 == memcmp (dataTest, data, dataTestLength));

        free (cb58Test);
        free (dataTest);
    }
}

static void
runAvalancheUtilsTest (void) {
    printf("TST:    Avalanche Utils\n");

    runAvalancheUtilsCB58Test ();

    return;
}

// MARK: - Hash Test

static void
runAvalancheHashTest (void) {
    static struct { char *data; /* ... */ } vectors[] = {
        { "Z8PYSYJHWJutjMDSdVBybSrFhmw1n8EYXEGb7drLg8mzrYEnq" },
        { NULL }
    };
    printf("TST:    Avalanche Hash\n");

    for (size_t index = 0; vectors[index].data != NULL; index++ ) {
    }
}

// MARK: - Address Test

static void
runAvalancheAddressTest (void) {
    static struct {
        const char * paperKey;
        const char * pubKey;
        const char * privKey;
        const char * ripemd160;
        const char * xaddress;
        const char * caddress;
    } vectors[] = {
        {
            //test account was made via :
            //https://iancoleman.io/bip39/ - bip32 seed phrase and decoded base58 encoded private seed to:
            //https://wallet.avax.network/access/privatekey
            "patient doctor olympic frog force glimpse endless antenna online dragon bargain someone",
            "029dc79308883267bb49f3924e9eb58d60bcecd17ad3f2f53681ecc5c668b2ba5f",
            "de7176242724956611e9a4f6dfb7a3b3b7eeeec0475b8bccdfec4e52a49c1466",
            "cc30e2015780a6c72efaef2280e3de4a954e770c",
            "avax1escwyq2hsznvwth6au3gpc77f225uacvwldgal",
            "bbc9bf879c06b13274c200c8b246881ef1ca33a0"
        },
        { NULL, NULL, NULL, NULL, NULL, NULL }
    };
    printf("TST:    Avalanche Address\n");

    for (size_t index = 0; vectors[index].paperKey != NULL; index++) {
        // 'raw'
        char addr_str[64]; size_t addr_len = 64;
        avax_addr_bech32_decode ((uint8_t *)addr_str, &addr_len, "avax", vectors[index].xaddress);
        addr_str[addr_len] = '\0';
        printf("TST:    Avalanche Address --- FAILED #1\n");
        //    assert (0 == strcmp (addr_str, "cc30e2015780a6c72efaef2280e3de4a954e770c"));
    
        UInt512 seed = UINT512_ZERO;
        BRBIP39DeriveKey(seed.u8, vectors[index].paperKey, NULL);
        BRAvalancheAccount account = avalancheAccountCreateWithSeed (seed);

        BRAvalancheAddress addressX = avalancheAccountGetAddress (account, AVALANCHE_CHAIN_TYPE_X);
        BRAvalancheAddress addressC = avalancheAccountGetAddress (account, AVALANCHE_CHAIN_TYPE_C);

        char *addressXString = avalancheAddressAsString (addressX);
        char *addressCString = avalancheAddressAsString (addressC);

        assert (0 == strcmp (addressXString, vectors[index].xaddress));
        assert (0 == strcmp (addressCString, vectors[index].caddress));

        printf("TST:    Avalanche Address --- FAILED #2\n");
        assert (avalancheAddressEqual (addressX, avalancheAddressCreateFromString(vectors[index].xaddress, true, AVALANCHE_CHAIN_TYPE_X)));
        assert (avalancheAddressEqual (addressC, avalancheAddressCreateFromString(vectors[index].caddress, true, AVALANCHE_CHAIN_TYPE_C)));

        avalancheAccountFree (account);
    }
}

// MARK: - Account Test

static void
runAvalancheAccountTest (void) {
    static struct {
        const char * paperKey;
    } vectors[] = {
        { "patient doctor olympic frog force glimpse endless antenna online dragon bargain someone" },
        { NULL }
    };

    printf("TST:    Avalanche Account\n");

    for (size_t index = 0; vectors[index].paperKey != NULL; index++) {
        UInt512 seed = UINT512_ZERO;
        BRBIP39DeriveKey(seed.u8, vectors[index].paperKey, NULL);
        BRAvalancheAccount account = avalancheAccountCreateWithSeed (seed);

        // Serialize
        size_t   accountSerializationSize;
        uint8_t *accountSerialization = avalancheAccountGetSerialization (account, &accountSerializationSize);

        // Deserialize
        BRAvalancheAmount accountRecovered = avalancheAccountCreateWithSerialization (accountSerialization,
                                                                                      accountSerializationSize);
        free (accountSerialization);

        assert (avalancheAddressEqual (avalancheAccountGetAddress (account, AVALANCHE_CHAIN_TYPE_X),
                                       avalancheAccountGetAddress (accountRecovered, AVALANCHE_CHAIN_TYPE_X)));

        assert (avalancheAddressEqual (avalancheAccountGetAddress (account, AVALANCHE_CHAIN_TYPE_C),
                                       avalancheAccountGetAddress (accountRecovered, AVALANCHE_CHAIN_TYPE_C)));

        assert (avalancheAccountHasAddress(accountRecovered,
                                           avalancheAccountGetAddress (account, AVALANCHE_CHAIN_TYPE_X)));

        assert (avalancheAccountHasAddress(accountRecovered,
                                           avalancheAccountGetAddress (account, AVALANCHE_CHAIN_TYPE_C)));

        int hasLimit = 0;
        assert (0 == avalancheAccountGetBalanceLimit (accountRecovered, 0, &hasLimit) && 0 == hasLimit);
        assert (0 == avalancheAccountGetBalanceLimit (accountRecovered, 1, &hasLimit) && 0 == hasLimit);

        avalancheAccountFree (account);
        avalancheAccountFree (accountRecovered);
    }
}

// MARK: - Fee Basis Test

static void
runAvalancheFeeBasisTest (void) {
    printf("TST:    Avalanche FeeBasis\n");
    return;
}

// MARK: - Transaction Test

static void
runAvalancheTransactionCreateTest (void) {
    printf("TST:        Avalanche Transaction Create\n");
    return;
}

static void
runAvalancheTransactionSignTest (void) {
    printf("TST:        Avalanche Transaction Sign\n");
   return;
}

static void
runAvalancheTransactionSerializeTest (void) {
    printf("TST:        Avalanche Transaction Serialize\n");
    return;
}

static void
runAvalancheTransactionTest (void) {
    printf("TST:    Avalanche Transaction\n");
    runAvalancheTransactionCreateTest ();
    runAvalancheTransactionSignTest ();
    runAvalancheTransactionSerializeTest ();
}

// MARK: - Wallet Test

static void
runAvalancheWalletTest (void) {
    printf("TST:    Avalanche Wallet\n");
   return;
}

// MARK: - All Tests

extern void
runAvalancheTest (void /* ... */) {
    printf("TST: Avalanche\n");

    runAvalancheUtilsTest ();
    runAvalancheHashTest ();
    runAvalancheAddressTest();
    runAvalancheAccountTest();
    runAvalancheFeeBasisTest ();
    runAvalancheTransactionTest ();
    runAvalancheWalletTest ();

    printf("TST: Avalanche Done\n");
}

