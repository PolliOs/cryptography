//
// Created by polli on 4/10/21.
//
#include "RSA.h"
#ifndef CRYPTOGRAPHY_RSA_CONNECT_H
#define CRYPTOGRAPHY_RSA_CONNECT_H

using namespace std;

struct Message{
    string author;
    vector<BigInt> content;

    Message(string author, vector<BigInt> content){
        this->author = author;
        this->content = content;
    }
};


class RSAClient {
    RSA rsa;
    RSA::Keys keys;
    string username;
    map<string, BigInt> modsForUser;

public:
    RSAClient(string username, int lengthInBits=32);
    string getUsername();
    BigInt getModulo();

    void updateModules(string username, BigInt mod, bool toAdd = true);
    void getMessage(Message msg);
    Message createMessage(string msg);

    vector<BigInt> encryptMessage(string msg);
    string decryptMessage(vector<BigInt> cipherText);

};

class RSAConnect{
    map<string, RSAClient*> clients;
public:
    static int publicExponent;
    bool join(RSAClient* client);
    void sendMessage(Message msg, RSAClient *pClient);

    vector<BigInt> getModules();

    void left(RSAClient *clientToLeave);
};


#endif //CRYPTOGRAPHY_RSA_CONNECT_H
