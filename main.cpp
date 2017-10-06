#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>


#include "hash.h"
#include "Block.h"
#include "common.h"
#include "BlockChain.h"


using namespace std;
/*
Hash header: index + prevHash + merkleRoot(data) + nonce
*/




class BlockChain {
    public:
        BlockChain();
        Block getBlock(int index);
        // getBlock(string hash); //not implemented
        int getNumOfBlocks(void);
        int addBlock(int index, string prevHash, string hash, string nonce, vector<string> &merkle);
        string getLatestBlockHash(void);
        void toString(void);
    private:
        vector<unique_ptr<Block> > blockchain;
};

BlockChain::BlockChain(){
    vector<string> v;
    v.push_back("Genesis Block!");
    // string header = to_string(0) + string("00000000000000") + getMerkleRoot(v);
    auto hash_nonce_pair = findHash(0,string("00000000000000"),v);
    
    this -> blockchain.push_back(std::make_unique<Block>(0,string("00000000000000"),hash_nonce_pair.first,hash_nonce_pair.second,v));
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
int BlockChain::getNumOfBlocks(void) {
    return this -> blockchain.size();
}
int BlockChain::addBlock(int index, string prevHash, string hash, string nonce, vector<string> &merkle) {
    string header = to_string(index) + prevHash + getMerkleRoot(merkle) + nonce;
    cout << header;
    if ( (!sha256(header).compare(hash)) && (hash.substr(0,2) == "00" )) {
        printf("\nBlock hashes match --- Adding Block %s \n",hash.c_str());
        this->blockchain.push_back(std::make_unique<Block>(index,prevHash,hash,nonce,merkle));
        return 1;
    }
    cout << "\nHash doesn't match criteria";
    return 0;
}
string BlockChain::getLatestBlockHash(void) {
    return this->blockchain[blockchain.size()-1]->getHash();
}
int main() {
    printf("Welcome! To quit-> Control c \n");
    char tmp[201];
    int ch;
    auto bc = BlockChain();
    for ( int i = 0; i < 10; i++ ) {
        vector<string> v;
        int temp;
        printf("(1) Look at Blocks \n(2) Add block\n");
        scanf("%d",&temp);
        if (temp == 1){
            printf("What Block do you want to look at? ");
            scanf("%d",&temp);
            try {
                bc.getBlock(temp).toString();
            }
            catch (const exception& e){
                cout << e.what() << "\n";
            }
        }
        else{
            printf("\nADDING BLOCKS!\nEnter your message: ");
            scanf("%200s",tmp);
            string str = tmp;
            printf("Entered '%s' into block\n",str.c_str());
            v.push_back(str);
        
            printf("Press any number to add block to blockchain:");
            scanf("%d",&ch);
        
            auto pair = findHash(bc.getNumOfBlocks(),bc.getLatestBlockHash(),v);
            bc.addBlock(bc.getNumOfBlocks(),bc.getLatestBlockHash(),pair.first,pair.second,v );
        }
    }
        
    

    // bc.addBlock(0,string("00000000000000"),string("003d9dc40cad6b414d45555e4b83045cfde74bcee6b09fb42536ca2500087fd9"),string("46"),v);
    printf("\n");
    return 0;
} 

