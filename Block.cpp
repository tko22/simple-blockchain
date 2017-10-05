#include <iostream>
#include "hash.h"
#include <vector>

using namespace std;

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

uint8_t getMerkleRoot(const vector<string> merkle) {
    // if (merkle.empty())
    //     return null;
    // else if (merkle.size() == 1)
    //     uint8_t digest[32];
    //     sha256(digest,merkle[0],strlen(merkle[0])
    //     return digest
    
    //     while (merkle.size() > 1) {
    //         if ( merkle.size() % 2 == 1 )
    //             merkle.push_back(merkle.back());


    //     }
    
}

// string Block::getTransactions(void)  {
//     return Block::transactions;
// }