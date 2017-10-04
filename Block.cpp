#include <iostream>
using namespace std;

class Block {
    public:
        Block(int prevHas, string trans  );
        int getPreviousHash(void);
        int getHash(void);
        hash<string> hash_fn;

        void toString(void);
    private:
        int previousHash;
        int blockHash;
        string transactions;
};
Block::Block( int prevHash, string trans ) {
    this -> previousHash = prevHash;
    this -> transactions = trans;
    int contents[] = {hash_fn(trans),previousHash};
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

// string Block::getTransactions(void)  {
//     return Block::transactions;
// }