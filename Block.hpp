//author: tko
#ifndef BLOCK_H
#define BLOCK_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

#include "json.hh"
using json = nlohmann::json;

class Block {
    public:
        Block(int index, string prevHas, string hash, string nonce, vector<string> data);
        string getPreviousHash(void);
        string getHash(void);
        int getIndex(void);
        vector<string> getData(void);

        void toString(void);
        json toJSON(void);
    private:
        int index;
        string previousHash;
        string blockHash;
        string nonce;
        vector<string> data;
        // string getMerkleRoot(const vector<string> &merkle);
};
// Constructor 
Block::Block(int index, string prevHash, string hash, string nonce, vector<string> data ) {
    printf("\nInitializing Block: %d ---- Hash: %s \n", index,hash.c_str());
    this -> previousHash = prevHash;
    this -> data = data;
    this -> index = index;
    this -> nonce = nonce;
    this -> blockHash = hash;
    
}

int Block::getIndex(void) {
    return this -> index;
}

string Block::getPreviousHash(void) {
    return this -> previousHash;
}

string Block::getHash(void) {
    return this -> blockHash;
}

vector<string> Block::getData(void){
    return this -> data;
}

// Prints Block data 
void Block::toString(void) {
    string dataString;
    for (int i=0; i < data.size(); i++)
        dataString += data[i] + ", ";
    printf("\n-------------------------------\n");
    printf("Block %d\nHash: %s\nPrevious Hash: %s\nContents: %s",
        index,this->blockHash.c_str(),this->previousHash.c_str(),dataString.c_str());
    printf("\n-------------------------------\n");
}

json Block::toJSON(void) {
    json j;
    j["index"] = this->index;
    j["hash"] = this->blockHash;
    j["previousHash"] = this->previousHash;
    j["nonce"] = this->nonce;
    j["data"] = this->data;
    return j;
}

#endif