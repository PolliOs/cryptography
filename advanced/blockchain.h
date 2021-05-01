//
// Created by polli on 4/23/21.
//

#ifndef CRYPTOGRAPHY_BLOCKCHAIN_H
#define CRYPTOGRAPHY_BLOCKCHAIN_H

#pragma once
#include <vector>
#include <map>
#include "RSA.h"
#include "sha256.h"
#include "merkleTree.h"
#include <nlohmann/json.hpp>


using json = nlohmann::json;


using namespace std;
using Address = BigInt;

struct RawTransaction {
    unsigned  int txId;
    Address from;
    Address to;
    double amount;
};

class Transaction{
protected:
    RawTransaction transaction;

public:
    RawTransaction getRawTransaction() const;
    string transactionToSign();
    void setRawTransaction(RawTransaction rawTransaction);
    virtual bool verify() = 0;
};

class SignedTransaction : public Transaction{
    RSA::Signature signature;
public:
    bool verify();
    void setSignature(RSA::Signature signature);
    RSA::Signature getSignature() const;
};


class MiningTransaction : public  Transaction{
public:
    MiningTransaction();
    MiningTransaction(Address addr);
    bool verify();
};

class Block{
public:
    struct Header {
        unsigned  int blockId;
        LongHex prevBlockHeaderHash;
        LongHex merkleRoot;
        unsigned int nonce;
    };

    static LongHex getMerkleRoot(vector<SignedTransaction>& signedTransaction, MiningTransaction miningTransaction);
    bool verify(LongHex prevBlockHeaderHash);
    LongHex getHeaderHash();
    Block::Header getHeader() const;
    void setHeader(Header header);
    void setTransactions(vector<SignedTransaction> signedTransactions, MiningTransaction miningTransaction);
    MiningTransaction getMiningTransaction() const;
    vector<SignedTransaction> getSignedTransactions() const;
    bool verifyMiningTransaction();
    void updateBalances(map<Address, double>& balances);

private:
    Header header;
    vector<SignedTransaction> transactions;
    MiningTransaction miningTransaction;
    bool verifyMerkleRoot();
};

class Blockchain {
    vector<Block> blocks;
    vector<SignedTransaction> pool;
    vector<map<Address, double>> balances;
public:
    void updateFile();
    const static int targetZerosPrefix;
    const static LongHex firstBlockHash;
    const static double reward;
    const static Address miningAddress;
    const static string file;
    static Blockchain load();
    void dump();

    LongHex getFirstBlockHash();
    vector<SignedTransaction> getPool() const;
    vector<Block> getBlocks() const;

    void setPool(vector<SignedTransaction> pool);
    void setBlocks(vector<Block> blocks);

    int getPoolSize();
    bool verify();
    bool addBlock(Block block);
    bool addSignedTransaction(SignedTransaction signedTransaction);
    LongHex getLastBlockHash();
    int getNumOfBlocks();

    map<Address, double> getBalances(int block = -1);
    double getBalance(Address address, int bllock = -1);


};

void to_json(json& j, const RawTransaction& rawTransaction);


void from_json(const json& j, RawTransaction& rawTransaction);

void to_json(json& j, const RSA::Signature& signature);
void from_json(const json& j, RSA::Signature& signature);
void to_json(json& j, const SignedTransaction& signedTransaction);
void from_json(const json& j, SignedTransaction& signed_transaction);

void to_json(json& j, const MiningTransaction& mining_transaction);
void from_json(const json& j, MiningTransaction& mining_transaction);

void to_json(json& j, const Block::Header& header);

void from_json(const json& j, Block::Header& header);

void to_json(json& j, const Block& block);

void from_json(const json& j, Block& block);
void to_json(json& j, const Blockchain& blockchain);
void from_json(const json& j, Blockchain& blockchain);

#endif //CRYPTOGRAPHY_BLOCKCHAIN_H
