#include <iostream>
#include "BigInt/BigInt.hpp"
#include "crypto.hpp"
#include "prime_rng.hpp"
#include "ElGamal.hpp"

void help();

void factor();

void log();

void phi();

void mu();

void legendre();

void jacobi();

void sqrt();

void prime();

void generate();

void ElGamalDemo();

void test();

void primeBPSW();

int main() {
    std::string command;
    help();
    while (true){
        std::cin >> command;

//        if (command == "help"){
//            help();
//        }
//
//        if (command == "factor"){
//            factor();
//        }
//
//        if (command == "log"){
//            log();
//        }
//
//        if (command == "phi"){
//            phi();
//        }
//
//        if (command == "mu"){
//            mu();
//        }
//
//        if (command == "legendre"){
//            legendre();
//        }
//
//        if (command == "jacobi"){
//            jacobi();
//        }
//
//        if (command == "sqrt"){
//            sqrt();
//        }

        if (command == "primeMR"){
            prime();
        }

        if (command == "primeBPSW"){
            primeBPSW();
        }
        if (command == "generate"){
            generate();
        }
        if (command == "test"){
            test();
        }

//        if (command == "ElGamal"){
//             ElGamalDemo();
//        }

        if (command == "exit"){
            return 0;
        }
    }
}

void assert(bool received, bool expected, std::string num){
    if(received != expected){
        std::cout << "assertion failed: number - " << num << "should is " << (expected?"prime": "composite") << " but another answer received\n";
    }
}

void test() {
    std::cout << "checking Miller–Rabin primality test\n";
    assert(Crypto::isPrime(BigInt("13")), true, "13");
    assert(Crypto::isPrime(BigInt("89")), true, "89");
    assert(Crypto::isPrime(BigInt("97")), true, "97");
    assert(Crypto::isPrime(BigInt("127")), true, "127");
    assert(Crypto::isPrime(BigInt("8191")), true, "8191");
    assert(Crypto::isPrime(BigInt("131071")), true, "131071");
    assert(Crypto::isPrime(BigInt("524287")), true, "524287");
    assert(Crypto::isPrime(BigInt("6700417")), true, "6700417");
    assert(Crypto::isPrime(BigInt("2147483647")), true, "2147483647");
    assert(Crypto::isPrime(BigInt("999999000001")), true, "999999000001");
    assert(Crypto::isPrime(BigInt("67280421310721")), true, "67280421310721");
    assert(Crypto::isPrime(BigInt("170141183460469231731687303715884105727")), true, "170141183460469231731687303715884105727");
    assert(Crypto::isPrime(BigInt("5210644015679228794060694325390955853335898483908056458352183851018372555735221")), true, "5210644015679228794060694325390955853335898483908056458352183851018372555735221");
    assert(Crypto::isPrime(BigInt("6864797660130609714981900799081393217269435300143305409394463459185543183397656052122559640661454554977296311391480858037121987999716643812574028291115057151")), true, "6864797660130609714981900799081393217269435300143305409394463459185543183397656052122559640661454554977296311391480858037121987999716643812574028291115057151");
    std::cout << "all test passed \n";

    std::cout << "checking Baillie–PSW primality test\n";
    assert(Crypto::isPrimeBPSW(BigInt("13")), true, "13");
    assert(Crypto::isPrimeBPSW(BigInt("79")), true, "79");
    assert(Crypto::isPrimeBPSW(BigInt("97")), true, "97");
    assert(Crypto::isPrimeBPSW(BigInt("107")), true, "107");
    assert(Crypto::isPrimeBPSW(BigInt("8191")), true, "8191");
    assert(Crypto::isPrimeBPSW(BigInt("131071")), true, "131071");
    assert(Crypto::isPrimeBPSW(BigInt("524287")), true, "524287");
    assert(Crypto::isPrimeBPSW(BigInt("6700417")), true, "6700417");
    assert(Crypto::isPrimeBPSW(BigInt("2147483647")), true, "2147483647");
    assert(Crypto::isPrimeBPSW(BigInt("999999000001")), true, "999999000001");
    assert(Crypto::isPrimeBPSW(BigInt("67280421310721")), true, "67280421310721");
    assert(Crypto::isPrimeBPSW(BigInt("170141183460469231731687303715884105727")), true, "170141183460469231731687303715884105727");
    assert(Crypto::isPrimeBPSW(BigInt("52106440156792287940606943253909558533358984839080564583521838510183725557352217")), true, "5210644015679228794060694325390955853335898483908056458352183851018372555735221");
    assert(Crypto::isPrimeBPSW(BigInt("6864797660130609714981900799081393217269435300143305409394463459185543183397656052122559640661454554977296311391480858037121987999716643812574028291115057151")), true, "6864797660130609714981900799081393217269435300143305409394463459185543183397656052122559640661454554977296311391480858037121987999716643812574028291115057151");
    std::cout << "all test passed \n";
    help();
}


void jacobi() {
    std::string a, n;
    std::cin >> a >> n;
    std::cout << Crypto::jacobi_symbol(BigInt(a), BigInt(n)) << std::endl;
}


void help() {
    std::cout << "Please enter command: \n"
//              << "factor x[factorize x]\n"
//              << "log a b n[solve a^x = b(mod n) for x]\n"
//              << "phi n[euler function]\n"
//              << "mu n[mobius function]\n"
//              << "legendre a p[calculates legendre symbol (a p)]\n"
//              << "jacobi a n[calculates jacobi symbol (a n)]\n"
//              << "sqrt a p[solves x^2=a(mod p) for x]\n"
              << "primeMR x[check if x is a prime number Miller-Rabin test]\n"
              << "primeBPSW x[check if x is a prime using BPSW test]\n"
              << "generate x[generate prime number with x bit, output in base2, base10 and base 64]\n"
              << "test[test on pre-setup cases]\n"
             // << "ElGamal\n"
              << "exit \n";
}
//32756834783245234533 example
void factor() {
    std::string n;
    std::cin >> n;
    auto factors = Crypto::factor(BigInt(n));
    for (auto f: factors){
        std::cout << f.first.toStr() << " " << f.second << std::endl;
    }
}


void log(){
    std::string a,b,n;
    std::cin >> a >> b >> n;
    std::cout << Crypto::discrete_log(
            BigInt(a), BigInt(b), BigInt(n)).toStr()
              << std::endl;
}

void phi() {
    std::string n;
    std::cin >> n;
    std::cout << Crypto::phi(BigInt(n)).toStr() << std::endl;
}


void mu() {
    std::string n;
    std::cin >> n;
    std::cout << Crypto::mu(BigInt(n)) << std::endl;
}

void legendre() {
    std::string a, n;
    std::cin >> a >> n;
    std::cout << Crypto::legendre_symbol(BigInt(a), BigInt(n)) << std::endl;
}

void sqrt(){
    std::string a,p;
    std::cin >> a >> p;
    std::cout << Crypto::discrete_sqrt(a, p).toStr() << std::endl;
}

void prime() {
    std::string n;
    std::cin >> n;
    bool ans = Crypto::isPrime(BigInt(n));
    if(ans) {
        std::cout << "yes\n";
    }else{
        std::cout << "no\n";
    }
}

void primeBPSW() {
    std::string n;
    std::cin >> n;
  //  std::cout << "here\n";
    bool ans = Crypto::isPrimeBPSW(BigInt(n));
    if(ans) {
        std::cout << "yes\n";
    }else{
        std::cout << "no\n";
    }
}


void generate() {
    int n;
    std::cin >> n;
    BigInt ans = Crypto::generatePrime(n);
    std::cout << ans.toStrBase2(n) << " " <<  ans.toStr() << " " << ans.toStrBase64() << "\n";
}


void ElGamalDemo() {
    ElGamal ElG = ElGamal();

    auto keys = ElG.createKeys();
    BigInt private_key = keys.first;
    auto public_key = keys.second;
    std::cout << "Keys art created\n";

    std::string msg, res = "";
    std::cout << "Enter message: \n";
    std::cin >> msg;

    for(char m:msg){
        auto c = ElG.encrypt_char(m, public_key);

        std::cout << "Encrypted point: \n"
                << "g = (" << c.first.x.toStr() << ", " << c.first.y.toStr() << ")\n"
                << "h = (" << c.second.x.toStr() << ", " << c.second.y.toStr() << ")\n";
        char decr = ElG.decrypt_char(private_key, c);
        res += decr;
    }

    ElG.assert(res == msg);

    std::cout << "Decrypted message: \n" << res << "\n";
}