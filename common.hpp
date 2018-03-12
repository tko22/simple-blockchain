// author: tko
#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

void print_hex(const char *label, const uint8_t *v, size_t len) {
    size_t i;

    printf("%s: ", label);
    for (i = 0; i < len; ++i) {
        printf("%02x", v[i]);
    }
    printf("\n");
}

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
#endif