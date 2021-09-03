//
//  WKWalletConnectorP.h
//  WalletKitCore
//
//  Created by Bryan Goring on 8/23/21
//  Copyright © 2021 Breadwinner AG. All rights reserved.
//
//  See the LICENSE file at the project root for license information.
//  See the CONTRIBUTORS file at the project root for a list of contributors.

#ifndef WKWalletConnectorP_h
#define WKWalletConnectorP_h

#include "WKWalletConnector.h"
#include "support/BRArray.h"

#ifdef __cplusplus
extern "C" {
#endif

// MARK: - Connector Handlers

typedef WKWalletConnector
(*WKWalletConnectorCreateHandler) (WKWalletManager manager);

typedef void
(*WKWalletConnectorReleaseHandler) (WKWalletConnector connector);

typedef uint8_t*
(*WKWalletConnectorGetDigestHandler) (
        WKWalletConnector       walletConnector,
        const uint8_t           *msg,
        size_t                  msgLen,
        WKBoolean               addPrefix,
        size_t                  *digestLength,
        WKWalletConnectorError  *err    );

typedef uint8_t*
(*WKWalletConnectorSignDataHandler) (
        WKWalletConnector       walletConnector,
        const uint8_t           *data,
        size_t                  dataLen,
        WKKey                   key,
        size_t                  *signatureLength,
        WKWalletConnectorError  *err    );

typedef uint8_t*
(*WKWalletConnectorCreateTransactionFromArgumentsHandler) (
        WKWalletConnector       connector,
        BRArrayOf (char*)       keys,
        BRArrayOf (char*)       values,
        size_t                  *serializationLength,
        WKWalletConnectorError  *err           );

typedef uint8_t*
(*WKWalletConnectorCreateTransactionFromSerialization) (
        WKWalletConnector       connector,
        const uint8_t           *data,
        size_t                  dataLength,
        size_t                  *signatureLength,
        WKBoolean               *isSigned,
        WKWalletConnectorError  *err            );

typedef struct {
    WKWalletConnectorCreateHandler                          create;
    WKWalletConnectorReleaseHandler                         release;
    WKWalletConnectorGetDigestHandler                       getDigest;
    WKWalletConnectorSignDataHandler                        sign;
    WKWalletConnectorCreateTransactionFromArgumentsHandler  createTransactionFromArguments;
    WKWalletConnectorCreateTransactionFromSerialization     createTransactionFromSerialization;
} WKWalletConnectorHandlers;

// MARK: - Connector

struct WKWalletConnectorRecord {
    WKNetworkType                       type;
    const WKWalletConnectorHandlers     *handlers;
    size_t                              sizeInBytes;

    WKWalletManager                     manager;
    // ...
};

private_extern WKWalletConnector
wkWalletConnectorAllocAndInit (size_t sizeInBytes,
                               WKNetworkType type,
                               WKWalletManager manager);

#ifdef __cplusplus
}
#endif

#endif /* WKWalletConnectorP_h */