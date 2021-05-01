//
// Created by polli on 4/23/21.
//

#include <fstream>
#include <iostream>
#include "blockchain.h"

using json = nlohmann::json;


void to_json(json& j, const RawTransaction& rawTransaction){
    j = json{
            {"txId", rawTransaction.txId},
            {"from", rawTransaction.from},
            {"to", rawTransaction.to},
            {"amount", rawTransaction.amount}

    };
}

void from_json(const json& j, RawTransaction& rawTransaction){
    j.at("txId").get_to(rawTransaction.txId);
    j.at("from").get_to(rawTransaction.from);
    j.at("to").get_to(rawTransaction.to);
    j.at("amount").get_to(rawTransaction.amount);
}

void to_json(json& j, const RSA::Signature& signature){
    j = json{{"signatures", signature.signatures},
             {"public_key", signature.publicKey},
             {"modulo", signature.mod}};
}

void from_json(const json& j, RSA::Signature& signature){
    j.at("signatures").get_to(signature.signatures);
    j.at("publicKey").get_to(signature.publicKey);
    j.at("modulo").get_to(signature.mod);
}

void to_json(json& j, const SignedTransaction& signedTransaction){
    j = json{{"rawTransaction", signedTransaction.getRawTransaction()},
             {"signature", signedTransaction.getSignature()}};
}

void from_json(const json& j, SignedTransaction& signedTransaction){
    RawTransaction rawtransaction;
    j.at("rawTransaction").get_to(rawtransaction);
    signedTransaction.setRawTransaction(rawtransaction);

    RSA::Signature signature;
    j.at("signature").get_to(signature);
    signedTransaction.setSignature(signature);
}

void to_json(json& j, const MiningTransaction& miningTransaction){
    j = json{{"rawTransaction", miningTransaction.getRawTransaction()}};
}

void from_json(const json& j, MiningTransaction& miningTransaction){
    RawTransaction rawTransaction;
    j.at("rawTransaction").get_to(rawTransaction);
    miningTransaction.setRawTransaction(rawTransaction);
}

void to_json(json& j, const Block::Header& header){
    j = json{{"block_id", header.blockId},
             {"previous_block_header_hash", header.prevBlockHeaderHash},
             {"merkle_root", header.merkleRoot},
             {"nonce", header.nonce}};
}

void from_json(const json& j, Block::Header& header){
    j.at("block_id").get_to(header.blockId);
    j.at("previous_block_header_hash").get_to(header.prevBlockHeaderHash);
    j.at("merkle_root").get_to(header.merkleRoot);
    j.at("nonce").get_to(header.nonce);
}

void to_json(json& j, const Block& block){
    j = json{{"header", block.getHeader()},
             {"signedTransactions", block.getSignedTransactions()},
             {"miningTransaction", block.getMiningTransaction()}};
}

void from_json(const json& j, Block& block){
    Block::Header header;
    j.at("header").get_to(header);
    block.setHeader(header);

    std::vector<SignedTransaction> signedTransactions;
    j.at("signedTransactions").get_to(signedTransactions);

    MiningTransaction miningTransaction;
    j.at("miningTransaction").get_to(miningTransaction);

    block.setTransactions(signedTransactions, miningTransaction);
}

void to_json(json& j, const Blockchain& blockchain){
    j = json{{"blocks", blockchain.getBlocks()},
             {"pool", blockchain.getPool()}};
}

void from_json(const json& j, Blockchain& blockchain){
    std::vector<Block> blocks;
    j.at("blocks").get_to(blocks);
    blockchain.setBlocks(blocks);

    std::vector<SignedTransaction> pool;
    j.at("pool").get_to(pool);
    blockchain.setPool(pool);
}


const int Blockchain::targetZerosPrefix = 2;
const LongHex Blockchain::firstBlockHash = LongHex();
const double Blockchain::reward = 10;
const Address Blockchain::miningAddress = Address();
const std::string Blockchain::file = "../blockchain.json";


void Blockchain::setPool(vector<SignedTransaction> pool) {
    this->pool = pool;
}

MiningTransaction::MiningTransaction(Address address) {
    RawTransaction rawTransaction;
    rawTransaction.from = Blockchain::miningAddress;
    rawTransaction.to = address;
    rawTransaction.amount = Blockchain::reward;
    this->setRawTransaction(rawTransaction);
}

void Transaction::setRawTransaction(RawTransaction transaction) {
    this->transaction = transaction;
}

RSA::Signature SignedTransaction::getSignature() const {
    return this->signature;
}

vector<SignedTransaction> Blockchain::getPool() const {
    return this->pool;
}

Block::Header Block::getHeader() const {
    return header;
}

LongHex Block::getMerkleRoot(vector<SignedTransaction> &signedTransactions, MiningTransaction miningTransaction) {
    vector<string> serializedTransactions;
    for(SignedTransaction signedTransaction: signedTransactions){
        json serializedTransaction = signedTransaction;
        serializedTransactions.push_back(serializedTransaction.dump());
    }
    json serializedTransaction = miningTransaction;
    serializedTransactions.push_back(serializedTransaction.dump());
    return MerkleTree::calculateRoot(serializedTransactions);
}

int Blockchain::getNumOfBlocks() {
    return this->blocks.size();
}

int Blockchain::getPoolSize() {
    return this->pool.size();
}

vector<SignedTransaction> Block::getSignedTransactions() const {
    return this->transactions;
}

LongHex Blockchain::getLastBlockHash() {
    return this->blocks.size() != 0 ? this->blocks.back().getHeaderHash() : this->getFirstBlockHash();
}

void Block::setHeader(Header header) {
    this->header = header;
}

LongHex Block::getHeaderHash(){
    SHA256 sha256;
    json header = this->getHeader();
    return sha256.hash(header.dump());
}

bool Block::verifyMiningTransaction() {
    return this->getHeaderHash().hasZerosPrefix(Blockchain::targetZerosPrefix);
}

bool Blockchain::addBlock(Block block) {
    if(!block.verify(this->getLastBlockHash()))
        return false;

    this->blocks.push_back(block);
    this->pool.clear();
    updateFile();
    return true;
}

void Blockchain::updateFile() {
    //cout << "in update";
    json blockchainJson = *this;
//    freopen("r", "blockchain.json", stdout);
//    dump_file.open();
//    if(!dump_file){
//        cout << "error opening the file!\n";
//    }
//    dump_file << blockchainJson.dump();
//    dump_file << "bla";
//    dump_file.close();
    ofstream outputFile(this->file);
    if (outputFile.fail())
    {
        cout << "Failed to open outputfile.\n";
    }
    outputFile << blockchainJson.dump();;
    outputFile.close();
   // cout << "finish";
//    freopen( "../blockchain.json","w", stdout);
//    cout << "HELLO!\n";



}

bool Block::verify(LongHex prevBlockHeaderHash) {
    if(this->getHeader().prevBlockHeaderHash != prevBlockHeaderHash){
        return false;
    }

    for(SignedTransaction transaction: transactions){
        if(!transaction.verify()){
            return false;
        }
    }
    return this->verifyMiningTransaction() && this->verifyMerkleRoot();
}

bool Block::verifyMerkleRoot() {
    return this->getHeader().merkleRoot == Block::getMerkleRoot(this->transactions, this->miningTransaction);
}


string Transaction::transactionToSign() {
    SHA256 sha256;
    RawTransaction rawTransaction = this->getRawTransaction();
    json rawTransactionJson = rawTransaction;
    return sha256.hash(rawTransactionJson.dump()).toStr();
}

MiningTransaction Block::getMiningTransaction() const {
    return this->miningTransaction;
}


MiningTransaction::MiningTransaction(){}

RawTransaction Transaction::getRawTransaction() const {
    return this->transaction;
}

void SignedTransaction::setSignature(RSA::Signature signature) {
    this->signature = signature;
}

bool Blockchain::addSignedTransaction(SignedTransaction signedTransaction) {
    if(!signedTransaction.verify()){
       // cout << "not verifyed!\n";
        return false;
    }
  //  cout << "pushed!\n";
    pool.push_back(signedTransaction);
    return true;
}

vector<Block> Blockchain::getBlocks() const {
    return this->blocks;
}

void Blockchain::setBlocks(vector<Block> blocks) {
    this->blocks = blocks;
}

bool SignedTransaction::verify() {
    RSA rsa;
    if(this->getRawTransaction().from == Blockchain::miningAddress){
        return false;
    }
    string rawTransactionHash = this->transactionToSign();
    return rsa.verify(rawTransactionHash, this->signature) && this->signature.publicKey == this->getRawTransaction().from;
}


bool Blockchain::verify() {
    LongHex prevBlockHeader = this->getFirstBlockHash();
    for(Block block:blocks){
        if(block.getHeader().prevBlockHeaderHash != prevBlockHeader){
            return false;
        }
        if(!block.verify(prevBlockHeader)){
            return false;
        }
        prevBlockHeader = block.getHeaderHash();
    }
    return true;
}

bool MiningTransaction::verify(){
    RawTransaction rawTransaction = this->getRawTransaction();
    return rawTransaction.amount == Blockchain::reward && rawTransaction.from == Blockchain::miningAddress;
}

LongHex Blockchain::getFirstBlockHash() {
    return this->firstBlockHash;
}

void Blockchain::dump(){
    json blockchainJson = *this;
    std::ofstream dump_file;
    dump_file.open(this->file);
    if(!dump_file){
        cout << "error opening the file!\n";
    }
    dump_file << blockchainJson.dump();
  //  cout << "dump";
    dump_file.close();
}


double Blockchain::getBalance(Address address, int block) {
    return this->getBalances(block)[address];
}

map<Address, double> Blockchain::getBalances(int block) {
    map<Address, double> balances;

    if(block == -1){
        block = getNumOfBlocks();
    }

    if(block > getNumOfBlocks()){
        return balances;
    }

    for(int i = 0; i < block; i++){
        this->blocks[i].updateBalances(balances);
    }

    return balances;
}


void Block::setTransactions(vector<SignedTransaction> signedTransactions, MiningTransaction miningTransaction) {
    this->transactions = signedTransactions;
    this->miningTransaction = miningTransaction;
}

void Block::updateBalances(map<Address, double> &balances) {
    auto amount = miningTransaction.getRawTransaction().amount;
   // cout << "in update --- amount = " << amount << "\n";
    balances[miningTransaction.getRawTransaction().to] +=  amount;


    for(SignedTransaction transaction: transactions){
        RawTransaction rawTransaction = transaction.getRawTransaction();
        amount = rawTransaction.amount;
        // cout << "curr amount = " << amount << "\n";
        balances[rawTransaction.from] -= amount;
        balances[rawTransaction.to] += amount;
    }
}

Blockchain Blockchain::load(){
    std::ifstream dump_file;
    dump_file.open(file);

    if (!dump_file.is_open()){
        cout << "can't open";
        return Blockchain();
    }

    json blockchain_json;
    dump_file >> blockchain_json;
    dump_file.close();

    auto blockchain = blockchain_json.get<Blockchain>();

    return blockchain;
}