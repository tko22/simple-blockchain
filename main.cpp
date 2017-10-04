#include <iostream>
#include <string>
#include "./Block.cpp"
// #include "hash.h"

using namespace std;

int main() {
    string str1 = "Hello";
    char s[] = "Hello";
    uint16_t prefix = 0xd17f;
    uint8_t suffix = 0x8c;
    uint8_t digest[32];
    uint8_t end[35];
    
    cout << s;

    // bbp_sha256(digest, )
    Block b1 = Block(0,str1);
    return 0;
} 