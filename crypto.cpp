//
// Created by polli on 11/29/20.
//

#include "crypto.hpp"
#include <iostream>

// Number is prime with probability 1-2^(-S) if isPrime
const int S = 50;
RNG Crypto::rng = RNG();


//Pollard's rho-algo
const BigInt findFactor(const BigInt& n){
    int max_iter = 1000, iter = 0;
    BigInt x_0 = 2;
    BigInt x_i = x_0, x_2i = x_0, d = BigInt(1);
    auto f = [n](const BigInt& x){return (x*x-1)%n;};
    while(iter < max_iter && (d == BigInt(1) || d == n)){
        d = gcd(n, abs(x_i - x_2i));
        x_i = f(x_i);
        x_2i = f(f(x_2i));
        iter++;
    }
    return d;
}

void factor(const BigInt& n, std::map<BigInt, int>& fs){
    if(n == 1) return;

    if(Crypto::isPrime(n)){
        auto pos = fs.find(n);
        if(pos != fs.end()){
            pos->second++;
        }else{
            fs[n] = 1;
        }
    }

    BigInt d = 1;
    while (d == 1 || d == n){
        d = findFactor(n);
    }

    factor(d, fs);
    factor(n/d, fs);
}

const BigInt trivial_factors(const BigInt& n, std::map<BigInt, int>& fs){
    BigInt m = n;
    for(BigInt i = 2; i < 10000 && i*i < n; i = i + 1) {
        if (m % i == 0) fs[i] = 0;
        while (m % i == 0) {
            fs[i]++;
            m = m / i;
        }
    }
    return m;
}

const std::map<BigInt, int> Crypto::factor(const BigInt &n) {
    auto factors = std::map<BigInt, int>();
    BigInt m = trivial_factors(n, factors);
    //std::cout << m.toStr() << " 1 \n";
    return factors;
}


//Baby-step-giant-step algo
const BigInt Crypto::discrete_log(const BigInt &a, const BigInt &b, const BigInt &n) {
    BigInt m = sqrt(n) + 1, p = 1;
    BigInt am = findInverse(pow(a,m,n), n);
    std::map<BigInt, BigInt> a_j = std::map<BigInt, BigInt>();

    a_j[1] = 0;
    for(BigInt j = 1; j < m; j = j + BigInt(1)){
        p = p * a % n;
        a_j[p] = j;

        if(p == b){
            return j;
        }
    }

    p = 1;
    for(BigInt i = 1; i < m; i = i + 1){
        p = p *am % n;
        auto pos = a_j.find(b*p%n);
        if(pos != a_j.end()){
            return pos->second + i*m;
        }
    }
    return -1;
}

const BigInt Crypto::phi(const BigInt &n) {
    auto factors = factor(n);
    BigInt res = BigInt(1);
    for (auto factor_pair: factors) {
        BigInt p = factor_pair.first, alpha = factor_pair.second, tmp;
        tmp = pow(p, alpha - 1, n);
        res = res * (p * tmp - tmp);
    }
    return res;
}

const int Crypto::mu(const BigInt& n){
    auto factors = factor(n);
    int res = factors.size() % 2 ? -1 : 1;
    for(auto factor_pair: factors){
        if(factor_pair.second > 1){
            return 0;
        }
    }
    return res;
}

const int Crypto::legendre_symbol(const BigInt& a, const BigInt& p){
    return pow(a, (p - 1)/2, p) == BigInt(1) ? 1 : -1;
}

const int Crypto::jacobi_symbol(const BigInt &a, const BigInt &m) {
    auto factors = factor(m);
    int res = 1;
    for(auto factor_pair: factors){
        BigInt p = factor_pair.first, alpha = factor_pair.second;
        if(alpha % 2 == 1){
            res *= legendre_symbol(a, p);
        }
    }
    return res;
}

//Cipolla's algo
const BigInt Crypto::discrete_sqrt(const BigInt &a, const BigInt &p) {
    BigInt w = 1, b = 0;
    while(legendre_symbol(w,p) == 1){
        b = rng.next(p);
        w = b*b - a;
        if(w == 0) return b;
    }

    BigInt deg = (p+1)/2;
    BigInt wk = 1, bk = pow(b,deg,p), ck = 1;
    BigInt res = (wk*ck*bk)%p;
    for(BigInt k = 2; k <= deg; k = k + 2){
        wk = (wk*w)%p;
        bk = pow(b, deg - k, p);
        ck = (ck*(deg - k + 1) * (deg - k + 2)) * findInverse((k-1)*k, p)%p;
        res = (res + ck*bk*wk)%p;
    }
    return res;
}

bool witness(const BigInt& a, const BigInt& n){
    BigInt t = 0, u = n-1;
    while(u%2 == 0){
        t = t + 1;
        u = u/2;
    }

    BigInt x_prev = pow(a,u,n), x;

    for(BigInt i = 0; i < t; i = i + 1){
        x = x_prev * x_prev % n;
        if(x == 1 && x_prev != 1 && x_prev != n-1){
            return true;
        }
        x_prev = x;
    }
    return x != 1;
}

//Miller-Rabin test
bool Crypto::isPrime(const BigInt &n) {
    if(n == 2 || n == 5 || n == 7) return true;
    if(n%2 == 0) return false;
    BigInt a;
    for(int i = 0; i < S; i++){
        a = rng.next(n-1) + 1;
        if(::witness(a,n)) return false;
    }
    return true;
}

// BPSW test
bool Crypto::isPrimeBPSW(const BigInt &n) {

    // first step Miller-Rabin
    if(!Crypto::isPrime(n)){
        return false;
    }

    //second step finding D
    BigInt a1 = BigInt("5"), a2 = BigInt("-4");
   // BigInt d;
    int numOfIter = 0;
    BigInt d = BigInt(5);
    int i = 0;
    while(d == 0 && numOfIter < 1000000){
        if(jacobi_symbol(a1, n) == -1){
            d = a1;
        }else if(jacobi_symbol(a2 , n) == -1){
            d = a2;
        }
        a1 = a1 + 4;
        a2 = a2 - 4;
        numOfIter++;
    }
 //   std::cout << "bla2\n";
    BigInt p = BigInt("1");
    BigInt q = (BigInt(1) - d) / BigInt(4);
   // std::cout << "d = " << d.toStr() << "p = " << p.toStr() << "q = " << q.toStr();
    return Crypto::lukeTest(n,d,p,q);
}

// Luke prime test
bool Crypto::lukeTest(const BigInt& n, const BigInt& d, const BigInt& p, const BigInt& q) {
    BigInt k = n, t, add;
    BigInt u = BigInt(1);
    BigInt v = BigInt(1);
    BigInt ind = BigInt(1);
    while (k > BigInt(0)){
        t = k % BigInt(2);
        k = k / BigInt(2);
        u = u*v;
        v = (v*v + d*u*u) / BigInt(2);
     //  v = (v*v - BigInt(2)*pow(q,ind,0));
       ind = ind + 1;


        if (t == BigInt(1)){
            add = p*u + v;

            if (add % BigInt(2) == BigInt(1))
                add = add + n;
            u = add / BigInt(2);
            add = d * u + p*v;

            if (add % BigInt(2) == BigInt(1))
                add = add + n;
            v = add / BigInt(2);
        }
        u = u % n;
        v = v % n;
    }
    add = p*u + v;
    if (add % BigInt(2) == BigInt(1))
        add = add + n;
    u = add / BigInt(2);
    return u % n == BigInt(0);
}

const BigInt Crypto::generatePrime(int length_in_bits){
        BigInt result = BigInt(4);
        while (!Crypto::isPrime(result))
            result = rng.next(length_in_bits);
        return result;
}