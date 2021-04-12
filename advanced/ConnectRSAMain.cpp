//
// Created by polli on 4/10/21.
//

#include "RSAConnect.h"
#include <iostream>

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

    RSAConnect rsaConnect;

    RSAClient* testA = new RSAClient("AliceTest",10);
    RSAClient* testB = new RSAClient("BobTest", 65);

    rsaConnect.join(testA);
    rsaConnect.join(testB);
    clock_t begin = clock();
    int numOfIterations = 10;
    freopen ("testOutput.txt","w",stdout);
    for(int i = 0; i < numOfIterations; i++) {
        rsaConnect.sendMessage(testA->createMessage("Hello, Bob!"), testB);
    }
    clock_t end = clock();
    double time1 = double(end - begin) /(numOfIterations* CLOCKS_PER_SEC);
    begin = clock();
    for(int i = 0; i < numOfIterations; i++) {
        rsaConnect.sendMessage(testB->createMessage("Hi, Alice!"), testA);
    }
    end = clock();
    double time2 = double(end - begin) /(numOfIterations* CLOCKS_PER_SEC);
    fclose (stdout);

    cout << time1 << "     " << time2;

}