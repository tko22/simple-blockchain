#include <iostream>
#include "hash.h"
#include <vector>

using namespace std;

class Block {
    public:
        Block(int prevHas, vector<string> trans  );
        string getPreviousHash(void);
        string getHash(void);

        void toString(void);
    private:
        string previousHash;
        string blockHash;
        vector<string> transactions;
};
Block::Block( string prevHash, vector<string> &trans ) {
    this -> previousHash = prevHash;
    this -> transactions = trans;

    int header[] = {hash_fn(trans),previousHash};
    // cout << "ContentS " << contents[1] << endl;
    // this -> blockHash = hash_fn(contents.to_bytes);
    // cout << blockHash;

}
string Block::getPreviousHash(void) {
    return previousHash;
}
string Block::getHash(void) {
    return blockHash;
}
void Block::toString(void) {
    cout << "Block hash: " << this -> blockHash << "\nprevHash: " << this -> previousHash;
}

string getMerkleRoot(const vector<string> &merkle) {
    if (merkle.empty())
        return "";
    else if (merkle.size() == 1){
        return sha256(merkle[0]);
    }

    vector<string> new_merkle = merkle;
    vector<string> result;
    
    cout << merkle.size();
    // while (new_merkle.size() > 1) {
    //     if ( new_merkle.size() % 2 == 1 )
    //         new_merkle.push_back(merkle.back());
    //         cout << new_merkle.end();
    //         // for (auto i = new_merkle.begin(); i != new_merkle.end(); i += 2){
    //         //     result = new_merkle.pop_back;
    //         // }
    //         new_merkle = result;
    //         cout << "done";

    // }
    return "string";
    
}

// string Block::getTransactions(void)  {
//     return Block::transactions;
// }