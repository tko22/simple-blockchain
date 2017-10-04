#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>

#include <openssl/sha.h>
#include <openssl/ripemd.h>
using namespace std;


void sha256(uint8_t *digest, const uint8_t *str, size_t length)
{
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str, length);
    SHA256_Final(digest, &sha256);

    
    // stringstream ss;
    // for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    // {
    //     ss << hex << setw(2) << setfill('0') << (int)hash[i];
    // }
    // return ss.str();
}

void print_hex(const char *label, const uint8_t *v, size_t len) {
    size_t i;

    printf("%s: ", label);
    for (i = 0; i < len; ++i) {
        printf("%02x", v[i]);
    }
    printf("\n");
}
