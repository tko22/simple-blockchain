#include <iostream>
#include <string>
#include "Block.h"
#include "hash.h"
#include <vector>
#include <memory>
#include <stdexcept>

using namespace std;
/*
Hash header: index + prevHash + merkleRoot(data) + nonce
*/



string getMerkleRoot(const vector<string> &merkle) {
    printf("\nFinding Merkle Root.... \n");
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
pair<string,string> findHash(int index, string prevHash, vector<string> &merkle) {
    string header = to_string(index) + prevHash + getMerkleRoot(merkle);
    unsigned int nonce;
    for (nonce = 0; nonce < 100000; nonce++ ) {
        string blockHash = sha256(header + to_string(nonce));
        if (blockHash.substr(0,2) == "00"){
            // cout << "nonce: " << nonce;
            // cout << "header: " << header;
            return make_pair(blockHash,to_string(nonce));
            break;
        }
    }
    return make_pair("fail","fail");
}
// int addBlock(int index, string prevHash, vector<string> &merkle, vector<unique_ptr<Block> > &blockchain) {
//     string header = to_string(index) + prevHash + getMerkleRoot(merkle);
//     auto pair = findHash(header);
    
//     blockchain.push_back(std::make_unique<Block>(index,prevHash,pair.first,pair.second,merkle));
//     return 1;
// }
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

