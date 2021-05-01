//
// Created by polli on 4/26/21.
//

#ifndef CRYPTOGRAPHY_ALPHABETAPROTOCOL_H
#define CRYPTOGRAPHY_ALPHABETAPROTOCOL_H

#include "BigInt.hpp"
#include "../crypto.hpp"


struct A {
    BigInt p;
    BigInt q;
};

struct B {
    BigInt z;
};

class AlphaBetaProtocol {
    BigInt n;
    A userA;
    B userB;

public:
    BigInt firstAlphaProcedure(BigInt s1, BigInt s2, int t);
    BigInt betaProcedure(BigInt s2);

    bool betaAuthentication(BigInt s1, BigInt s2, int t);

    bool alphaAuthentication(int t);

    bool sendAndVerify(BigInt a);

    BigInt solveCRT(std::vector<BigInt> vector, std::vector<BigInt> vector1);
};


#endif //CRYPTOGRAPHY_ALPHABETAPROTOCOL_H
