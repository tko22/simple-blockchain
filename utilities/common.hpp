#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <iomanip>
#include <sstream>
#include <openssl/evp.h>

void print_hex(const char* label, const uint8_t* v, size_t len)
{
    std::cout << label << ": ";
    for (size_t i = 0; i < len; ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0')
                  << static_cast<int>(v[i]);
    }
    std::cout << "\n";
}

std::string sha256(const std::string& str)
{
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int lengthOfHash = 0;

    EVP_MD_CTX* sha256 = EVP_MD_CTX_new();
    if (!sha256)
        throw std::runtime_error("Failed to create SHA256 context");
    if (EVP_DigestInit_ex(sha256, EVP_sha256(), nullptr) != 1) {
        EVP_MD_CTX_free(sha256);
        throw std::runtime_error("Failed to initialize SHA256 digest");
    }
    if (EVP_DigestUpdate(sha256, str.c_str(), str.size()) != 1) {
        EVP_MD_CTX_free(sha256);
        throw std::runtime_error("Failed to update SHA256 digest");
    }
    if (EVP_DigestFinal_ex(sha256, hash, &lengthOfHash) != 1) {
        EVP_MD_CTX_free(sha256);
        throw std::runtime_error("Failed to finalize SHA256 digest");
    }
    EVP_MD_CTX_free(sha256);

    std::stringstream ss;
    for (unsigned int i = 0; i < lengthOfHash; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}

std::string getMerkleRoot(const std::vector<std::string>& merkle)
{
    std::cout << "\nFinding Merkle Root.... \n";
    if (merkle.empty())
        return "";
    if (merkle.size() == 1)
        return sha256(merkle[0]);

    std::vector<std::string> new_merkle = merkle;
    while (new_merkle.size() > 1) {
        if (new_merkle.size() % 2 == 1)
            new_merkle.push_back(
              new_merkle.back()); // Ensures even number of elements

        std::vector<std::string> result;
        for (size_t i = 0; i < new_merkle.size(); i += 2) {
            std::string hash =
              sha256(sha256(new_merkle[i]) + sha256(new_merkle[i + 1]));
            result.push_back(hash);
        }
        new_merkle = result;
    }
    return new_merkle.front();
}

std::pair<std::string, std::string> findHash(
  int index,
  const std::string& prevHash,
  const std::vector<std::string>& merkle)
{
    std::string header =
      std::to_string(index) + prevHash + getMerkleRoot(merkle);
    for (unsigned int nonce = 0; nonce < 100000; ++nonce) {
        std::string blockHash = sha256(header + std::to_string(nonce));
        if (blockHash.substr(0, 2) == "00") {
            return { blockHash, std::to_string(nonce) };
        }
    }
    return { "fail", "fail" };
}