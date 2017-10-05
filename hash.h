#ifndef _HASH_H_
#define _HASH_H_
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>

#include <openssl/sha.h>
#include <openssl/ripemd.h>
using namespace std;


string sha256(const uint8_t *str, size_t length) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str, length);
    SHA256_Final(hash, &sha256);
    
    stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }

    return ss.str();
}

#endif