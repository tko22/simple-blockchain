//author: tko
#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <iostream>
#include <string>
#include "hash.hpp"
#include <vector>
#include <memory>
#include <stdexcept>

#include "json.hh"
using json = nlohmann::json;

class BlockChain {
public:
    BlockChain(int genesis  = 1 );
    Block getBlock(int index);
    // getBlock(string hash); //not implemented
    int getNumOfBlocks(void);
    int addBlock(int index, string prevHash, string hash, string nonce, vector<string> &merkle);
    string getLatestBlockHash(void);
    // void toString(void);
    string toJSON(void);
    int replaceChain(json chain);
private:
    vector<unique_ptr<Block> > blockchain; //vector that is the blockchain
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
// Gets block based on the index
Block BlockChain::getBlock(int index) {
    for ( int i = 0; i <blockchain.size(); i++ ){
        if (blockchain[i]->getIndex() == index) {
            return *(blockchain[i]);
        }
    }
    throw invalid_argument("Index does not exist.");
}

// returns number of blocks
int BlockChain::getNumOfBlocks(void) {
    return this -> blockchain.size();
}

// checks whether data fits with the right hash -> add block
int BlockChain::addBlock(int index, string prevHash, string hash, string nonce, vector<string> &merkle) {
    string header = to_string(index) + prevHash + getMerkleRoot(merkle) + nonce;
    if ( (!sha256(header).compare(hash)) && (hash.substr(0,2) == "00" ) && (index == blockchain.size())) {
        printf("Block hashes match --- Adding Block %s \n",hash.c_str());
        this->blockchain.push_back(std::make_unique<Block>(index,prevHash,hash,nonce,merkle));
        return 1;
    }
    cout << "Hash doesn't match criteria\n";
    return 0;
}

// returns hash of the latest block, used for finding the previousHash when mining new block
string BlockChain::getLatestBlockHash(void) {
    return this->blockchain[blockchain.size()-1]->getHash();
}

// returns JSON string of JSON - used to send to network
string BlockChain::toJSON() {
    json j;
    j["length"] = this->blockchain.size();
    for (int i = 0; i < this->blockchain.size(); i++){
        j["data"][this->blockchain[i]->getIndex()] = this->blockchain[i]->toJSON();
    }
    return j.dump(3);
}

// replaces Chain with new chain represented by a JSON, used when node sends new blockchain
int BlockChain::replaceChain(json chain) {
    //remove all blocks except for the first block
    while (this->blockchain.size() > 1){
        this->blockchain.pop_back();
    }
    for (int a = 1; a <chain["length"].get<int>(); a++ ){
        auto block = chain["data"][a];
        vector<string> data = block["data"].get<vector<string> >();
        this->addBlock(block["index"],block["previousHash"],block["hash"],block["nonce"],data);
    } 
    return 1;
}

#endif