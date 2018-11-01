# simple-blockchain

My implementation of a blockchain in C++ I created for fun :)

_Note that this was written before knowing more about distributed system concepts. Use at your own discretion._

Follows some Bitcoin design principles including a peer-to-peer network, SHA-256 to hash headers and blocks, merkle trees, and "mining"(more on that below).

#### Requirements

- Uses C++14, OpenSSL library, Simple-Web-Server, and a [JSON library](https://github.com/nlohmann/json)

Includes a Command line interface that allows you to view blockchains at different indices and add new blocks. You
can do that 20 times until it automatically quits but you can change that. Control-c to quit.

And unfortunately, everything is stored in memory and is deleted when program quits.

## Peer-to-Peer Network

At first, I used WebSockets but a peer-to-peer system would require setting up a WS server and WS clients for each and every node.

So instead, I make HTTP requests to connect to nodes to the network

#### For this to work we need to:

- Keep track of nodes in the network
- Get the latest chains from every node -- to validate your chain & get up to date when a new node is added to the network
- Send out your chain to the network when a new block is added

#### Conflicts in different chains

There can only be one explicit set of blocks in the chain at a given time. If there are conflicts(e.g. when the chains at different nodes have the same size but have different blocks), the longest chain is chosen.

So if some other node sends in a new chain that's longer than yours, your chain is replaced.

**Note:** this was a simple implementation and thus, it replaces the entire chain except for the genesis block. For future improvements, each node should check the new chain with other nodes before it is added and entire nodes shouldn't be sent out. I didn't have access to other computers in the same network, so the nodes are connected through different ports inside your computer.

## BlockChain Object

#### Private Variables:

- blockchain(vector<unique_ptr<Block> >): vector of smart pointers to Block objects

Genesis Block is created during intialization.

#### Validating the integrity of blocks

Every time you want to add a block to the blockchain, you will need to provide it:

`int index, string prevHash, string hash, string nonce, vector<string> &merkle`

- index: index of the block
- prevHash: hash of the previous block
- nonce: self-explantory
- merkle: vector holding in the data of the block

It will then check whether you have the correct hash(it rehashes the information given), if you have "00" in front, and whether your index is correct.

Note: this is the only way to add to the blockchain.

## Mining for I guess... Proof of Stake

(I made it very simple because I didn't want to spend much processing power so it's honestly not proof of stake but I just added it in for fun) - use findHash() to get hash and nonce

first two characters of the hash must be 0

- e.g. `003d9dc40cad6b414d45555e4b83045cfde74bcee6b09fb42536ca2500087fd9` works

## Block Object

Hash header: index + prevHash + merkleRoot(data) + nonce

#### Private Variables:

- index
- Data: vector of strings
- previousHash
- blockHash
- nonce

For a block to be immutable, its properties are private and there are only methods that return them but not update them.

## Common Functions

#### getMerkleRoot(const vector<string> &merkle)

- gets merkle root based on elements of a vector

#### findHash(int index, string prevHash, vector<string> &merkle)

- "Mining" part
- finds hash and returns a std::pair of the hash found and nonce used to find it

### Author

tk2@illinois.edu
