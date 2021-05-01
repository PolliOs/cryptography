#pragma once
#include "BigInt.hpp"
#include "rng.hpp"
#include <map>

class Crypto {
private:
	static RNG rng;
public:

	// solve equation g^x = b (mod n)
	static const BigInt discrete_log(const BigInt& g, const BigInt& b, const BigInt& n);

	// return factors as a map (p_i, alpha_i)
	static const std::map<BigInt, int> factor(const BigInt& n);
	
	static bool isPrime(const BigInt& n);

	// calculates Euler's function
	static const BigInt phi(const BigInt& n);

	// calculates Mobius function
	static const int mu(const BigInt& n);

	static const int legendre_symbol(const BigInt& a, const BigInt& p);
	static const int jacobi_symbol(const BigInt& a, const BigInt& p);

	// solve equation x^2 = a (mod n)
	static const BigInt discrete_sqrt(const BigInt& a, const BigInt& p);

	static const BigInt generatePrime(int len_in_bits);

    static const BigInt generateRNG(int len_in_bits);

    static const BigInt generatePrime(int len_in_bits, BigInt s);

    static bool isPrimeBPSW(const BigInt &n);


    static bool lukeTest(const BigInt &n, const BigInt &d, const BigInt &p, const BigInt &q);
};