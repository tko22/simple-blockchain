#include <iostream>
#include <string>
#include "Block.cpp"
#include "hash.h"


using namespace std;

int main() {

    char s[] = "Hello";
    uint16_t prefix = 0xd17f;
    uint8_t suffix = 0x8c;
    uint8_t digest[32];
    uint8_t SecondDigest[32];
    uint8_t end[35];
    sha256(digest, (uint8_t *)s, strlen(s));
    sha256(SecondDigest, (uint8_t *)s, strlen(s));

    print_hex("SHA256",digest,32);
    print_hex("SHA256",SecondDigest,32);
    // sha256()
    Block b1 = Block(0,"hello");
    return 0;
} 