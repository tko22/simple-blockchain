#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <stdexcept>

#include "../utilities/common.hpp"
#include "Block.hpp"

class Blockchain
{
public:
    explicit Blockchain(bool createGenesis = true);
    Block getBlock(int index) const;
    std::size_t getNumOfBlocks() const;
    bool addBlock(std::size_t index,
                  const std::string& prevHash,
                  const std::string& hash,
                  const std::string& nonce,
                  const std::vector<std::string>& merkle);
    std::string getLatestBlockHash() const;
    std::string toJSON() const;
    bool replaceChain(const json& chain);

private:
    std::vector<std::unique_ptr<Block>> m_blocks;
};

// If integer passed into constructor is false,
// it is the first node and creates the genesis block
Blockchain::Blockchain(bool createGenesis)
{
    if (!createGenesis) {
        std::vector<std::string> genesisData = { "Genesis Block!" };
        auto hash_nonce_pair = findHash(0, "00000000000000", genesisData);
        m_blocks.push_back(std::make_unique<Block>(0,
                                                   "00000000000000",
                                                   hash_nonce_pair.first,
                                                   hash_nonce_pair.second,
                                                   genesisData));
        std::cout << "Created Blockchain with Genesis Block!\n";
    }
}
// Gets block based on the index
Block Blockchain::getBlock(int index) const
{
    auto it = std::find_if(
      m_blocks.begin(), m_blocks.end(), [index](const auto& block) {
          return block->getIndex() == index;
      });
    if (it == m_blocks.end())
        throw std::invalid_argument("Index does not exist.");
    return **it;
}

// returns number of blocks
std::size_t Blockchain::getNumOfBlocks() const
{
    return m_blocks.size();
}

// checks whether data fits with the right hash -> add block
bool Blockchain::addBlock(std::size_t index,
                          const std::string& prevHash,
                          const std::string& hash,
                          const std::string& nonce,
                          const std::vector<std::string>& merkle)
{
    std::string header =
      std::to_string(index) + prevHash + getMerkleRoot(merkle) + nonce;
    if (sha256(header) == hash && hash.substr(0, 2) == "00" &&
        index == m_blocks.size()) {
        m_blocks.push_back(
          std::make_unique<Block>(index, prevHash, hash, nonce, merkle));
        std::cout << "Block hashes match --- Adding Block " << hash << "\n";
        return true;
    }
    std::cout << "Hash doesn't match criteria\n";
    return false;
}

// returns hash of the latest block, used for finding the previousHash when
// mining new block
std::string Blockchain::getLatestBlockHash() const
{
    return m_blocks.back()->getHash();
}

// returns JSON string of JSON - used to send to network
std::string Blockchain::toJSON() const
{
    json j;
    j["length"] = m_blocks.size();
    for (const auto& block : m_blocks) {
        j["data"][block->getIndex()] = block->toJSON();
    }
    return j.dump(3);
}

// replaces Chain with new chain represented by a JSON, used when node sends new
// Blockchain
bool Blockchain::replaceChain(const json& chain)
{
    m_blocks.erase(m_blocks.begin() + 1, m_blocks.end());
    for (int i = 1; i < chain["length"].get<int>(); ++i) {
        const auto& block = chain["data"][i];
        std::vector<std::string> data =
          block["data"].get<std::vector<std::string>>();
        if (!addBlock(block["index"],
                      block["previousHash"],
                      block["hash"],
                      block["nonce"],
                      data))
            return false;
    }
    return true;
}