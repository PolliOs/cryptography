//
// Created by polli on 4/10/21.
//

#include <iostream>
#include "RSAConnect.h"

int RSAConnect::publicExponent = 65537;

bool RSAConnect::join(RSAClient *newClient) {
    for(auto existingClient:clients) {
        if (gcd(existingClient.second->getModulo(), newClient->getModulo()) != 1) {
            return false;
        }
    }
    clients[newClient->getUsername()] = newClient;

    for (auto existingClient: clients) {
        existingClient.second->updateModules(newClient->getUsername(), newClient->getModulo(), true);
    }

    return true;
}


void RSAConnect::sendMessage(Message msg, RSAClient *clientWhoSends) {
    for(auto client:clients){
        if(client.second != clientWhoSends) {
            client.second->getMessage(msg);
        }
    }
}

void RSAConnect::sendSilentMessage(Message msg, RSAClient *clientWhoSends) {
    for(auto client:clients){
        if(client.second != clientWhoSends) {
            client.second->getSilenttMessage(msg);
        }
    }
}

RSAClient::RSAClient(string username, int lengthInBits) {
    this->username = username;
    this->keys = rsa.createKeys(lengthInBits, RSAConnect::publicExponent);
}

void RSAClient::updateModules(string username, BigInt mod, bool toAdd) {
    if(toAdd){
        this->modsForUser[username] = mod;
    }else {
        this->modsForUser.erase(username);
    }
}

string RSAClient::getUsername() {
    return this->username;
}

BigInt RSAClient::getModulo() {
    return this->keys.n;
}

void RSAClient::getMessage(Message msg) {
    string message = BigIntConverter::toStr(msg.content);
    auto signature = this->rsa.sign(message, this->keys);
    string decrypted = this->decryptMessage(msg.content);
    cout << "User: " << this->getUsername() << " received message: " << decrypted
    << " from user: " << msg.author << "\n";
    if(!this->rsa.verify(decrypted, signature)){
        cout << "Verify failed!\n";
    }
}

void RSAClient::getSilenttMessage(Message msg) {
    auto username = this->getUsername();
    auto receivedMsg = this->decryptMessage(msg.content);
    auto authoer = msg.author;
}

Message RSAClient::createMessage(string msg) {
    return Message(this->getUsername(), this->encryptMessage(msg));
}

const BigInt chineseRemaindersTheorem(std::vector<BigInt> remainders, std::vector<BigInt> modules) {
    int n = remainders.size();
    vector<vector<BigInt>> inverse(n);

    for(int i = 0; i < n; i++){
        for(int j = i; j < n; j++){
            inverse[i].push_back(findInverse(modules[i], modules[j]));
        }
    }

    BigInt res(0), mult(1);
    vector<BigInt> x(n);

    for(int i = 0; i < n; i++){
        x[i] = remainders[i];
        for(int j = 0; j < i; j++){
            x[i] = (x[i] - x[j]) * inverse[j][i];
            x[i] = (x[i] + modules[i]) % modules[i];
        }
        res = res + mult*x[i];
        mult = mult * modules[i];
    }
    return res;

}


vector<BigInt> RSAClient::encryptMessage(string msg) {
    vector<BigInt> modules;
    for(auto elem: modsForUser){
        modules.push_back((elem.second));
    }

    vector<vector<BigInt>> encryptedMsgByMod;
    for(auto mod:modules){
        encryptedMsgByMod.push_back(rsa.encrypt(msg, keys.publicKey, mod));
    }

    vector<BigInt> remainders;
    vector<BigInt> encryptedForAll;
    for(int chunk = 0; chunk < encryptedMsgByMod[0].size(); chunk++){
        remainders.clear();
        for(int client = 0; client < modules.size(); client++){
            remainders.push_back(encryptedMsgByMod[client][chunk]);
        }
        encryptedForAll.push_back(chineseRemaindersTheorem(remainders, modules));
    }

    return encryptedForAll;
}

string RSAClient::decryptMessage(vector<BigInt> cipherText) {
    return rsa.decrypt(cipherText, keys.privateKey, keys.n);
}

void RSAConnect::left(RSAClient* clientToLeave){
    for (auto clientEntry: clients){
        clientEntry.second->updateModules(clientToLeave->getUsername(), clientToLeave->getModulo(), false);
    }
    clients.erase(clientToLeave->getUsername());
}