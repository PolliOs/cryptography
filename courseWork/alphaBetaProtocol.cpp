//
// Created by polli on 4/26/21.
//

#include "alphaBetaProtocol.h"
#include <vector>

BigInt AlphaBetaProtocol::firstAlphaProcedure(BigInt s1, BigInt s2, int t) {
    this->userA.p = Crypto::generatePrime(t, s1);
    this->userA.q = Crypto::generatePrime(t, s2);
    this->n = userA.p * userA.q;
    BigInt z = (findInverse(s2, s1) * findInverse(userA.q, s1)) % s1;
    return z;
}

BigInt AlphaBetaProtocol::betaProcedure(BigInt s2) {
    BigInt w = s2 * this->userB.z;
    return  w;
}

bool AlphaBetaProtocol::betaAuthentication(BigInt s1, BigInt s2, int t) {
    this->userB.z = firstAlphaProcedure(s1,s2,t);
    BigInt w = betaProcedure(s2);
    BigInt p = (n * w) % s1;
    BigInt mod = n % p;
    if(mod == BigInt(0)){
        return true;
    }
    return false;
}


bool AlphaBetaProtocol::alphaAuthentication(int t) {
    BigInt x = Crypto::generateRNG(t);
    BigInt x_2 = x*x;
    BigInt a = x_2 % n;
    bool verifiedX = sendAndVerify(a);
    return verifiedX;
}

bool AlphaBetaProtocol::sendAndVerify(BigInt a) {
    std::vector<BigInt> remainders;

    BigInt rem1 = sqrt(a % this->userA.p), rem2 = this->userA.p - rem1;
    BigInt rem3 = sqrt(a % this->userA.q), rem4 = this->userA.q - rem3;

    std::vector<BigInt>modules;
    modules.push_back(BigInt(this->userA.p));
    modules.push_back(BigInt(this->userA.q));
    remainders.push_back(rem1);
    remainders.push_back(rem3);
    BigInt res1 = solveCRT(remainders, modules);

    remainders[1] = rem4;
    BigInt res2 = solveCRT(remainders, modules);

    remainders[0] = rem2;
    BigInt res3 = solveCRT(remainders, modules);

    remainders[1] = rem3;
    BigInt res4 = solveCRT(remainders, modules);

    return res1 != res2 && res1 != res3 && res1 != res4 && res2 != res3 && res2 != res4 && res3 != res4;
}

BigInt AlphaBetaProtocol::solveCRT(std::vector<BigInt> remainders, std::vector<BigInt> modules) {
    BigInt prod = BigInt(1), ans = BigInt(0);
    std::vector<BigInt> m(remainders.size());

    for (int i = 0; i < modules.size(); i++)
        prod = prod * modules[i];
    for (int i = 0; i < m.size(); i++) {
        m[i] = prod / modules[i];

        ans = ans + (((remainders[i] * m[i]) % prod) * binPow(m[i] % modules[i], modules[i] - 2, modules[i])) % prod;
        ans = ans % prod;
    }
    return ans;
}