//
// Created by polli on 4/24/21.
//

#include <iostream>
#include "blockchainClient.h"

BlockchainClient::BlockchainClient() {
    this->blockchain = Blockchain::load();
}

bool BlockchainClient::mineBlock(Address address) {
    MiningTransaction miningTransaction(address);
    vector<SignedTransaction> pool = blockchain.getPool();
    LongHex merkleRoot = Block::getMerkleRoot(pool, miningTransaction);

    Block::Header header;
    header.blockId =blockchain.getNumOfBlocks();
    header.merkleRoot = merkleRoot;
    header.prevBlockHeaderHash = blockchain.getLastBlockHash();

    Block block;
    block.setTransactions(pool, miningTransaction);

    for(unsigned int nonce = 0; nonce < UINT_MAX; nonce++){
        header.nonce = nonce;
        block.setHeader(header);
        if(block.verifyMiningTransaction()){
            return blockchain.addBlock(block);
        }
    }
    return false;
}

void BlockchainClient::addSignedTransaction(RSA::Keys from, Address to, double amount) {
    SignedTransaction signedTransaction;

    RawTransaction rawTransaction;
    rawTransaction.txId = blockchain.getPoolSize();
    //cout << "txId = " << rawTransaction.txId << "\n";
    rawTransaction.amount = amount;
    rawTransaction.from = from.publicKey;
    rawTransaction.to = to;

    signedTransaction.setRawTransaction(rawTransaction);
    string  rawTransactionHash = signedTransaction.transactionToSign();
    RSA rsa;
    signedTransaction.setSignature(rsa.sign(rawTransactionHash, from));

    blockchain.addSignedTransaction(signedTransaction);
}

bool BlockchainClient::verifyBlockchain() {
    return this->blockchain.verify();
}

double BlockchainClient::getBalance(Address address, int block) {
    return this->blockchain.getBalance(address, block);
}
std::map<Address, double> BlockchainClient::getBalances(int block){
    return blockchain.getBalances(block);
}

void BlockchainClient::dump(){
    blockchain.dump();
}