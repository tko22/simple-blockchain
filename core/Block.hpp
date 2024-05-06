#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

#include "../utilities/json.hh"
using json = nlohmann::json;

class Block
{
public:
    Block(int idx,
          const std::string& prevHash,
          const std::string& hash,
          const std::string& nonce,
          const std::vector<std::string>& data);
    std::string getPreviousHash() const;
    std::string getHash() const;
    int getIndex() const;
    std::vector<std::string> getData() const;

    void toString() const;
    json toJSON() const;

private:
    int index;
    std::string previousHash;
    std::string blockHash;
    std::string nonce;
    std::vector<std::string> data;
};

Block::Block(int idx,
             const std::string& prevHash,
             const std::string& hash,
             const std::string& nonce,
             const std::vector<std::string>& data)
  : index(idx)
  , previousHash(prevHash)
  , blockHash(hash)
  , nonce(nonce)
  , data(data)
{
    std::cout << "\nInitializing Block: " << index << " ---- Hash: " << hash
              << std::endl;
}

int Block::getIndex() const
{
    return index;
}

std::string Block::getPreviousHash() const
{
    return previousHash;
}

std::string Block::getHash() const
{
    return blockHash;
}

std::vector<std::string> Block::getData() const
{
    return data;
}

void Block::toString() const
{
    std::string dataString =
      std::accumulate(begin(data),
                      end(data),
                      std::string(),
                      [](const std::string& a, const std::string& b) {
                          return a.empty() ? b : a + ", " + b;
                      });
    std::cout << "\n-------------------------------\n"
              << "Block " << index << "\nHash: " << blockHash
              << "\nPrevious Hash: " << previousHash
              << "\nContents: " << dataString
              << "\n-------------------------------\n";
}

json Block::toJSON() const
{
    json j;
    j["index"] = index;
    j["hash"] = blockHash;
    j["previousHash"] = previousHash;
    j["nonce"] = nonce;
    j["data"] = data;
    return j;
}