
#include <iostream>
#include <string>
// #include "Block.h"
#include "hash.h"
#include <vector>
#include <memory>



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
        Block(int index, string prevHas, vector<string> trans  );
        string getPreviousHash(void);
        string getHash(void);
        vector<string> getTransaction(void);

        void toString(void);
    private:
        int index;
        string previousHash;
        string blockHash;
        vector<string> transactions;
        string getMerkleRoot(const vector<string> &merkle);
};
Block::Block(int index, string prevHash, vector<string> trans ) {
    printf("Initializing Block....\n\n");
    this -> previousHash = prevHash;
    this -> transactions = trans;
    this -> index = index;
    string header = getMerkleRoot(trans) + previousHash;
    this -> blockHash = sha256(header);
        
}
string Block::getPreviousHash(void) {
    return this -> previousHash;
}
string Block::getHash(void) {
    return this -> blockHash;
}
vector<string> Block::getTransaction(void){
    return this -> transactions;
}
void Block::toString(void) {
    cout << "Block hash: " << this -> blockHash << "prevHash: " << this -> previousHash;
}

string Block::getMerkleRoot(const vector<string> &merkle) {
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
            printf("---hash(hash(%s), hash(%s)) => %s\n",new_merkle[0].c_str(),new_merkle[1].c_str(),hash.c_str());
            result.push_back(hash);
        }
        new_merkle = result;
    }
    printf("\n");
    return new_merkle[0];

}

// string Block::getTransactions(void)  {
//     return Block::transactions;
// }

int main() {
       
    vector<unique_ptr<Block> > blockchain; 

    vector<string> v;
    string str = "hello";
    string third = "third";
    string fourth = "fourth";
    string s = "Hello Bitcoin!"; 
    v.push_back(s);
    v.push_back(str);
    v.push_back(string("init string"));
    v.push_back(string("final string"));

    blockchain.push_back(std::make_unique<Block>(0,string("0"),v));
    cout << blockchain[0]->getHash();
    // unique_ptr<Block> b1 = unique_ptr<Block>(new Block(1,*genesis.getHash(),v));
    printf("\n");

    return 0;
} 

