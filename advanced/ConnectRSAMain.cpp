//
// Created by polli on 4/10/21.
//

#include "RSAConnect.h"
#include <iostream>
#include <fstream>

void test();

int main(){
    RSAConnect rsaConnect;

    RSAClient* alice = new RSAClient("Alice",10);
    RSAClient* bob = new RSAClient("Bob", 65);

    rsaConnect.join(alice);
    rsaConnect.join(bob);
    rsaConnect.sendMessage(alice->createMessage("Hello, Bob!"), alice);

    rsaConnect.sendMessage(bob->createMessage("Hi, Alice!"), alice);

    test();
    return 0;
}

void test() {
    cout << "in test \n";

    RSAConnect rsaConnect2;

    RSAClient* testA = new RSAClient("AliceTest",10);
    RSAClient* testB = new RSAClient("BobTest", 65);

    rsaConnect2.join(testA);
    rsaConnect2.join(testB);
    clock_t begin = clock();
    int numOfIterations = 10;
    for(int i = 0; i < numOfIterations; i++) {
        rsaConnect2.sendSilentMessage(testA->createMessage("Hello, Bob!"), testB);
    }
    clock_t end = clock();
    double time1 = double(end - begin) / CLOCKS_PER_SEC;
    begin = clock();
    for(int i = 0; i < numOfIterations; i++) {
        rsaConnect2.sendSilentMessage(testB->createMessage("Hi, Alice!"), testA);
    }
    end = clock();
    double time2 = double(end - begin) /( CLOCKS_PER_SEC);

    cout << time1 << "     " << time2;

}