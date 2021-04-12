//
// Created by polli on 4/10/21.
//
#include "../crypto.hpp"
#include <vector>
#pragma once
#ifndef CRYPTOGRAPHY_RSA_H
#define CRYPTOGRAPHY_RSA_H


using namespace std;

class RSA {
    vector<BigInt> encrypt_(vector<BigInt> msg, const BigInt& publicKey,const BigInt& mod);
    vector<BigInt> decrypt_(vector<BigInt> ciphertext, const BigInt& privateKey, const BigInt& mod);

public:
    struct Keys {
        BigInt privateKey;
        BigInt publicKey;
        BigInt n;
    };

    struct Signature {
        vector<BigInt> signatures;
        BigInt publicKey;
        BigInt mod;
    };

    vector<BigInt> encrypt(string msg, const BigInt& publicKey, const BigInt& mod);
    string decrypt(vector<BigInt> ciphertext, const BigInt& privateKey, const BigInt& mod);

    const BigInt encrypt(const BigInt& msg, const BigInt& publicKey, const BigInt& mod);
    const BigInt decrypt(const BigInt& ciphertext, const BigInt& privateKey, const BigInt& mod);

    Keys createKeys(int lengthInBits, int publicExponentVal = 65537);

    RSA::Signature sign(string& msg, RSA::Keys keys);
    bool verify(string& msg, RSA::Signature signature);
};

class BigIntConverter{
public:
    static vector<BigInt> toVector(string msg);
    static string toStr(vector<BigInt> vec);
};


#endif //CRYPTOGRAPHY_RSA_H
