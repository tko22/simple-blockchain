#include <iostream>
#include <string>
// #include "Block.cpp"
#include "hash.h"
#include <vector>


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
        Block(int prevHas, vector<string> trans  );
        int getPreviousHash(void);
        int getHash(void);

        void toString(void);
    private:
        int previousHash;
        int blockHash;
        vector<string> transactions;
};
Block::Block( int prevHash, vector<string> trans ) {
    this -> previousHash = prevHash;
    this -> transactions = trans;
    // int contents[] = {hash_fn(trans),previousHash};
    // cout << "ContentS " << contents[1] << endl;
    // this -> blockHash = hash_fn(contents.to_bytes);
    // cout << blockHash;

}
int Block::getPreviousHash(void) {
    return previousHash;
}
int Block::getHash(void) {
    return blockHash;
}
void Block::toString(void) {
    cout << "Block hash: " << this -> blockHash << "prevHash: " << this -> previousHash;
}

string getMerkleRoot(const vector<string> &merkle) {
    if (merkle.empty())
        return "";
    else if (merkle.size() == 1){
        return sha256(merkle[0]);
    }

    vector<string> new_merkle = merkle;
    vector<string> result;

    while (new_merkle.size() > 1) {
        if ( new_merkle.size() % 2 == 1 )
            new_merkle.push_back(merkle.back());

        for (auto it= new_merkle.begin(); it != new_merkle.end(); it += 2){
            string var1 = sha256(*it);
            string var2 = sha256(*(it + 1));

            result.push_back(sha256(var1 + var2));
        }
        new_merkle = result;
    }
    return result[0];

}

// string Block::getTransactions(void)  {
//     return Block::transactions;
// }

int main() {
    string s = "Hello Bitcoin!";    

    // uint8_t digest[32];
    string digest = sha256(s);
    printf("\n");

    vector<string> v;
    string str = "hello";
    v.push_back(s);
    v.push_back(str);
    
    cout << getMerkleRoot(v);
    // Block b1 = Block(0,v);
    printf("\n");
    return 0;
} 

