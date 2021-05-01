//
// Created by polli on 4/26/21.
//

#include "alphaBetaProtocol.h"
#include <iostream>

using namespace std;

int main(){
    AlphaBetaProtocol abProtocol;
    int t = 32;
    //std::cin >> t;
    BigInt s1 = Crypto::generatePrime(t);
    BigInt s2 = Crypto::generatePrime(t);
    while(gcd(s1,s2) != 1){
        s2 = Crypto::generatePrime(t);
    }
    std::cout << "s1 = " << s1.toStr() << "\ns2 = " << s2.toStr() << "\n";
    bool betaAuth = abProtocol.betaAuthentication(s1, s2, t);
    cout << (betaAuth ? "Success authentication of B\n" : "Authentication of B failed\n");
    bool alphaAuth = abProtocol.alphaAuthentication(t);
    cout << (alphaAuth ? "Success authentication of A\n" : "Authentication of A failed\n");

    return 0;
}



