#include <iostream>
#include <string>
// #include "Block.h"
#include "hash.h"
#include <vector>
#include <memory>
#include <stdexcept>

using namespace std;

void print_hex(const char *label, const uint8_t *v, size_t len) {
    size_t i;

    printf("%s: ", label);
    for (i = 0; i < len; ++i) {
        printf("%02x", v[i]);
    }
    printf("\n");
}

class Block {
    public:
        Block(int index, string prevHas, string hash, string nonce, vector<string> data  );
        string getPreviousHash(void);
        string getHash(void);
        int getIndex(void);
        vector<string> getData(void);

        void toString(void);
    private:
        int index;
        string previousHash;
        string blockHash;
        string nonce;
        vector<string> data;
        // string getMerkleRoot(const vector<string> &merkle);
};
Block::Block(int index, string prevHash, string hash, string nonce, vector<string> data ) {
    printf("Initializing Block: %d ---- Hash: %s ", index,hash.c_str());
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
void Block::toString(void) {
    cout << "Block hash: " << this -> blockHash << "prevHash: " << this -> previousHash;
}

string getMerkleRoot(const vector<string> &merkle) {
    printf("Finding Merkle Root.... \n");
    if (merkle.empty())
        return "";
    else if (merkle.size() == 1){
        return sha256(merkle[0]);
    }

    vector<string> new_merkle = merkle;

    while (new_merkle.size() > 1) {
        if ( new_merkle.size() % 2 == 1 )
            new_merkle.push_back(merkle.back());

        vector<string> result;
            
        for (int i=0; i < new_merkle.size(); i += 2){
            string var1 = sha256(new_merkle[i]);
            string var2 = sha256(new_merkle[i+1]);
            string hash = sha256(var1+var2);
            // printf("---hash(hash(%s), hash(%s)) => %s\n",new_merkle[0].c_str(),new_merkle[1].c_str(),hash.c_str());
            result.push_back(hash);
        }
        new_merkle = result;
    }
    return new_merkle[0];

}
pair<string,string> findHash(string header) {
    unsigned int nonce;
    for (nonce = 0; nonce < 100000; nonce++ ) {

        string blockHash = sha256(header + to_string(nonce));
        if (blockHash.substr(0,2) == "00"){
            return make_pair(blockHash,to_string(nonce));
            break;
        }
    }
    return make_pair("fail","fail");
}
int addBlock(int index, string prevHash, vector<string> &merkle, vector<unique_ptr<Block> > &blockchain) {
    string header = to_string(index) + prevHash + getMerkleRoot(merkle);
    auto pair = findHash(header);
    
    blockchain.push_back(std::make_unique<Block>(index,prevHash,pair.first,pair.second,merkle));
    return 1;
}
class BlockChain {
    public:
        BlockChain();
        Block getBlock(int index);
        // getBlock(string hash); //not implemented
        int getNumOfBlocks(void);
        int addBlock(void);
        void toString(void);
    private:
        vector<unique_ptr<Block> > blockchain;
};
BlockChain::BlockChain(){
    vector<string> v;
    v.push_back("Genesis Block!");
    string header = string(0) + string("00000000000000") + getMerkleRoot(v);
    auto hash_nonce_pair = findHash(header);

    this -> blockchain.push_back(std::make_unique<Block>(index,string("00000000000000"),hash_nonce_pair.first,hash_nonce_pair.second,v));
    printf("Created blockchain!\n");
}
Block BlockChain::getBlock(int index) {
    for ( int i = 0; i <blockchain.size(); i++ ){
        if (blockchain[i]->getIndex() == index) {
            return *(blockchain[i]);
        }
    }
    throw invalid_argument("Index does not exist.");
}
int main() {
    vector<unique_ptr<Block> > blockchain; 

    BlockChain();
    printf("Blockchains!!! Enter your message:");


    
    

    printf("\n");
    return 0;
} 

