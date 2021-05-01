//
// Created by polli on 4/24/21.
//

#include "blockchainClient.h"
#include<map>
#include<iostream>

void help();

int main(){
    BlockchainClient blockchainClient;
    map<string, RSA::Keys> account;
    map<string, BigInt> to;
    string alias;
    RSA rsa;
    help();

    string command;
    while (cin >> command){

        if (command == "create"){
            cout << "alias: ";
            cin >> alias;
            account[alias] = rsa.createKeys(32, 0);
            to[alias] = account[alias].publicKey;
        }

        if (command == "mine"){
            cout << "alias: ";
            cin >> alias;
            if (blockchainClient.mineBlock(account[alias].publicKey)) {
                cout << "\nSuccess.";
            } else {
                cout << "\nFailed.";
            }
        }

        if (command == "add"){
            string accFrom, accTo;
            double amount;
            cout << "Enter account aliases from, to and amount\n";
            cin >> accFrom >> accTo >> amount;
            blockchainClient.addSignedTransaction(account[accFrom], to[accTo], amount);
        }

        if (command == "verify"){
            if (blockchainClient.verifyBlockchain()){
                cout << "Verified. Data is consistent.";
            } else {
                cout << "Data is inconsistent";
            }
        }

        if (command == "set"){
            cout << "Enter alias and address\n";
            string address;
            cin >> alias >> address;
            to[alias] = BigInt(address);
        }

        if (command == "balance"){
            int block;
            cout << "Enter alias and block ";
            cin >> alias >> block;
            cout << blockchainClient.getBalance(to[alias], block);
        }

        if (command == "balances"){
            int block;
            cout << "Enter block ";
            cin >> block;
            auto result = blockchainClient.getBalances(block);

            for (auto pos = result.begin(); pos != result.end(); pos++){
                cout << "Address: " << (*pos).first.toStr() << ". Balance: "  << (*pos).second << "\n";
            }
        }

        if (command == "list"){
            for (auto pos = account.begin(); pos != account.end(); pos++){
                cout << (*pos).first << " address: " << (*pos).second.publicKey.toStr() << "\n";
            }
        }

        if (command == "help"){
           help();
        }

        if (command == "exit"){
            blockchainClient.dump();
            return 0;
        }

        cout << "\n";
    }
}

void help() {
    cout << "create : add new address\n";
    cout << "mine : mine next block\n";
    cout << "list : list all account aliases\n";
    cout << "add : create transaction\n";
    cout << "verify : verify blockchain\n";
    cout << "balance : get balance\n";
    cout << "balances : get balances\n";
    cout << "help\n";
    cout << "exit : exit client\n";

}
