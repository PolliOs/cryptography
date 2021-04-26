//
// Created by polli on 4/24/21.
//

#ifndef CRYPTOGRAPHY_BLOCKCHAINCLIENT_H
#define CRYPTOGRAPHY_BLOCKCHAINCLIENT_H

#include "blockchain.h"

class BlockchainClient {
    Blockchain blockchain;
public:
    BlockchainClient();
    bool mineBlock(Address address);
    void addSignedTransaction(RSA::Keys from, Address to, double amount);
    bool verifyBlockchain();
    double getBalance(Address address, int block);
    void dump();
    map<Address, double> getBalances(int block);
};



#endif //CRYPTOGRAPHY_BLOCKCHAINCLIENT_H
