#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <iostream>
#include <string>
#include "hash.h"
#include <vector>
#include <memory>
#include <stdexcept>

class BlockChain {
public:
    BlockChain(int genesis  = 1 );
    Block getBlock(int index);
    // getBlock(string hash); //not implemented
    int getNumOfBlocks(void);
    int addBlock(int index, string prevHash, string hash, string nonce, vector<string> &merkle);
    string getLatestBlockHash(void);
    void toString(void);
private:
    vector<unique_ptr<Block> > blockchain;
};

// If integer passed into constructor is 0, it the first node and creates the genesis block
BlockChain::BlockChain(int genesis ){
    if (genesis == 0) {
        vector<string> v;
        v.push_back("Genesis Block!");
        // string header = to_string(0) + string("00000000000000") + getMerkleRoot(v);
        auto hash_nonce_pair = findHash(0,string("00000000000000"),v);
    
        this -> blockchain.push_back(std::make_unique<Block>(0,string("00000000000000"),hash_nonce_pair.first,hash_nonce_pair.second,v));
        printf("Created blockchain!\n");
    }
    
}

Block BlockChain::getBlock(int index) {
    for ( int i = 0; i <blockchain.size(); i++ ){
        if (blockchain[i]->getIndex() == index) {
            return *(blockchain[i]);
        }
    }
    throw invalid_argument("Index does not exist.");
}

int BlockChain::getNumOfBlocks(void) {
    return this -> blockchain.size();
}

int BlockChain::addBlock(int index, string prevHash, string hash, string nonce, vector<string> &merkle) {
    string header = to_string(index) + prevHash + getMerkleRoot(merkle) + nonce;
    cout << header;
    if ( (!sha256(header).compare(hash)) && (hash.substr(0,2) == "00" ) && (index == blockchain.size())) {
        printf("\nBlock hashes match --- Adding Block %s \n",hash.c_str());
        this->blockchain.push_back(std::make_unique<Block>(index,prevHash,hash,nonce,merkle));
        return 1;
    }
    cout << "\nHash doesn't match criteria\n";
    return 0;
}

string BlockChain::getLatestBlockHash(void) {
    return this->blockchain[blockchain.size()-1]->getHash();
}

#endif