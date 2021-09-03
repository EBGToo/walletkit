/*
 * Created by Bryan Goring <bryan.goring@breadwallet.com> on 08/24/21.
 * Copyright (c) 2021 Breadwinner AG.  All right reserved.
 *
 * See the LICENSE file at the project root for license information.
 * See the CONTRIBUTORS file at the project root for a list of contributors.
 */
package com.blockset.walletkit.nativex;

public enum WKWalletConnectorError {

    /** Cannot support WalletConnect on this network
     *
     */
    UNSUPPORTED_CONNECTOR {
        @Override
        public int toCore() {
            return UNSUPPORTED_CONNECTOR_VALUE;
        }
    },

    /** Indicates that the digest is invalid or failed to be produced.
     *
     */
    INVALID_DIGEST {
        @Override
        public int toCore() {
            return 0; // TODO align with finalized WKWalletConnector.h error defs
        }
    },

    /** Indicates that the signature is invalid or failed to be completed.
     */
    INVALID_SIGNATURE {
        @Override
        public int toCore() {
            return 0; // TODO align with finalized WKWalletConnector.h error defs
        }
    },

    /** Indicates that the serialization returned is invalid or failed to process.
     */
    INVALID_SERIALIZATION {
        @Override
        public int toCore() {
            return 0; // TODO align with finalized WKWalletConnector.h error defs
        }
    };

    public static WKWalletConnectorError fromCore(int nativeValue) {
        switch (nativeValue) {
            case UNSUPPORTED_CONNECTOR_VALUE:         return UNSUPPORTED_CONNECTOR;
            default: throw new IllegalArgumentException("Invalid WKWalletConnectorError core value");
        }
    }



    /** Indicates whether the native value is actually representative
     *  of an error.
     *
     *  This check relies knowledge
     * @param nativeValue The native code to be examined
     * @return True if this indeed is an error.
     */
    public static boolean isAnError(int nativeValue) {
        return nativeValue != WK_WALLET_CONNECTOR_ERROR_IS_UNDEFINED;
    }

    /** Represents a define held within native WalletConnect error definitions, and
     *  reproduced here.
     */
    private static final int WK_WALLET_CONNECTOR_ERROR_IS_UNDEFINED = -1;

    // Valid WalletConnect native errors follow
    private static final int UNSUPPORTED_CONNECTOR_VALUE          = 1;

    public abstract int toCore();
}