//
// Created by polli on 4/23/21.
//

#include "merkleTree.h"

SHA256* MerkleTree::hasher = new SHA256();

LongHex MerkleTree::calculateRoot(vector<string> &inputs) {
    if(inputs.empty()) return LongHex();

    vector<vector<LongHex>> state(2);

    int curr = 0;
    for(auto input:inputs){
        state[curr].push_back(hasher->hash(input));
    }

    while(state[curr].size() != 1){
        for(int i = 0; i < state[curr].size(); i+= 2){
            LongHex nodeVal = state[curr][i];
            nodeVal = nodeVal.concatenate((i+1 < state[curr].size()) ? state[curr][i+1] : nodeVal);
            state[1-curr].push_back(hasher->hash(nodeVal));
        }
        state[curr].clear();
        curr = 1 - curr;
    }

    return state[curr][0];
}