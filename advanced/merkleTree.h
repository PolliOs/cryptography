//
// Created by polli on 4/23/21.
//

#ifndef CRYPTOGRAPHY_MERKLETREE_H
#define CRYPTOGRAPHY_MERKLETREE_H

#include <vector>
#include "sha256.h"

using namespace std;

class MerkleTree {
    static SHA256* hasher;

public:
    static LongHex calculateRoot(vector<string>& inputs);
};


#endif //CRYPTOGRAPHY_MERKLETREE_H
