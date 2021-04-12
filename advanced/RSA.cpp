//
// Created by polli on 4/10/21.
//

#include "RSA.h"

using namespace std;
RSA::Keys RSA::createKeys(int lengthInBits, int publicExponentVal) {
    BigInt p = Crypto::generatePrime(lengthInBits);
    BigInt q = Crypto::generatePrime(lengthInBits);

    BigInt n = p * q;
    BigInt phiN = (p - 1) * (q - 1);
    BigInt carmichaelN =  phiN / gcd((p - 1), (q - 1));

    BigInt publicExponent = publicExponentVal == 0 ? Crypto::generatePrime(lengthInBits / 2):BigInt(publicExponentVal);

    BigInt privateExponent = findInverse(publicExponent, carmichaelN);

    RSA::Keys res;

    res.privateKey = privateExponent;
    res.publicKey = publicExponent;
    res.n = n;
    return  res;
}


const BigInt RSA::encrypt(const BigInt &msg, const BigInt &publicKey, const BigInt &mod) {
    return pow(msg, publicKey, mod);
}

const BigInt RSA::decrypt(const BigInt& ciphertext, const BigInt &privateKey, const BigInt &mod) {
    return pow(ciphertext, privateKey, mod);
}


vector<BigInt> RSA::encrypt_(vector<BigInt> msg, const BigInt &publicKey, const BigInt &mod) {
    vector<BigInt> resultEncryption;
    for(auto s: msg){
        resultEncryption.push_back(encrypt(s, publicKey, mod));
    }
    return resultEncryption;
}

vector<BigInt> RSA::decrypt_(vector<BigInt> ciphertext, const BigInt &privateKey, const BigInt &mod) {
    vector<BigInt> resultDecryption;
    for(auto s:ciphertext){
        resultDecryption.push_back(decrypt(s, privateKey, mod));
    }
    return resultDecryption;
}

vector<BigInt> RSA::encrypt(string msg, const BigInt &publicKey, const BigInt &mod) {
    return encrypt_(BigIntConverter::toVector(msg), publicKey, mod);
}

string RSA::decrypt(vector<BigInt> ciphertext, const BigInt &privateKey, const BigInt &mod) {
    return BigIntConverter::toStr(decrypt_(ciphertext, privateKey, mod));
}

vector<BigInt> BigIntConverter::toVector(string msg) {
    vector<BigInt> resultVector;
    for(char c: msg){
        resultVector.push_back(BigInt(int(c)));
    }
    return resultVector;
}

string BigIntConverter::toStr(vector<BigInt> vec) {
    string resultString = "";
    for(BigInt elem: vec){
        resultString += char(elem.firstDigit());
    }
    return resultString;
}

RSA::Signature RSA::sign(string &msg, RSA::Keys keys) {
    Signature signature;
    signature.publicKey = keys.publicKey;
    signature.mod = keys.n;
    vector<BigInt> convertedMessage = BigIntConverter::toVector(msg);
    signature.signatures = encrypt_(convertedMessage, keys.privateKey, signature.mod);
    return signature;
}

bool RSA::verify(string &msg, RSA::Signature signature) {
    return BigIntConverter::toStr(decrypt_(signature.signatures, signature.publicKey, signature.mod)) == msg;
}
