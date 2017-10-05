#include <iostream>
#include <string>
#include "Block.cpp"
#include "hash.h"


using namespace std;

void print_hex(const char *label, const uint8_t *v, size_t len) {
    size_t i;

    printf("%s: ", label);
    for (i = 0; i < len; ++i) {
        printf("%02x", v[i]);
    }
    printf("\n");
}

int main() {
    char s[] = "Hello Bitcoin!";    

    // uint8_t digest[32];
    string digest = sha256((uint8_t *)s, strlen(s));
    cout << digest;
    printf("\n");
    // print_hex("SHA256",SecondDigest,32);
    vector<string> v;
    string str = "hello";
    v.push_back(str);
    Block b1 = Block(0,v);
    printf("\n");
    return 0;
} 

