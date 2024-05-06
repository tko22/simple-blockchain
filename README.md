# Simple Blockchain

This is implementation of a blockchain in C++, created as a fun educational project.

_Note: This project was developed with a basic understanding of distributed system concepts. It's primarily for learning purposes and not optimized for production use._

This blockchain model incorporates elements from Bitcoin's design, including a peer-to-peer network architecture, SHA-256 for hashing, Merkle trees, and a simplified form of "mining".

### Requirements

- **C++17**: Ensure your compiler supports at least C++17.
- **OpenSSL Library**: For SHA-256 hashing.
- **Simple-Web-Server**: For handling HTTP requests between nodes.
- **JSON Library**: [nlohmann/json](https://github.com/nlohmann/json) for data serialization.

The system includes a Command Line Interface (CLI) that allows interaction with the blockchain, such as viewing and adding blocks. The CLI permits up to 20 operations before it exits automatically, but this can be adjusted as needed. Use Control-C to quit the program at any time. Note that all data is held in memory and will be lost when the program is terminated.

## Peer-to-Peer Network

Initially considered using WebSockets, I opted for HTTP requests due to the simplicity of integrating HTTP servers and clients without the overhead of setting up WebSocket servers and clients for each node.

### Networking Details:

- **Node Tracking**: Maintains a list of other nodes in the network.
- **Chain Syncing**: Retrieves the latest blockchain from every node to validate and update the local chain when a new node joins.
- **Chain Broadcasting**: Distributes the current blockchain to other nodes whenever a new block is added.

### Conflict Resolution

The blockchain enforces a singular, authoritative chain at any time. Conflicts (e.g., when two nodes have chains of the same length but with different blocks) are resolved by adopting the longer chain. If a received chain is longer than the current chain, the local chain is updated accordingly.

**Note**: This implementation simplifies network interactions and assumes all nodes are on the same local network but listening on different ports.

## Blockchain Object

### Components:

- **Blocks**: Managed via a vector of unique pointers to `Block` objects.
- **Genesis Block**: Automatically created upon initialization.

### Block Validation

To add a block, you must provide:
- `index`: Block position in the chain.
- `prevHash`: Hash of the preceding block.
- `hash`: Computed SHA-256 hash of the new block.
- `nonce`: A nonce that, combined with the above details, leads to a hash with specific characteristics (e.g., starting with "00").
- `merkle`: Vector of strings containing the block's data.

Each addition is validated against these criteria to ensure chain integrity.

## Simplified Mining (Proof of Concept)

This version includes a basic "mining" process using `findHash()` to generate a valid hash and nonce. The hash must start with "00" to be accepted.

## Block Object

### Structure:

- **Index**: Position of the block in the chain.
- **Data**: Contents of the block, stored as a vector of strings.
- **Hashes**: Includes both the previous and current block hashes.
- **Nonce**: Used to calculate the current block's hash.

Blocks are designed to be immutable, with private properties only accessible through getter methods.

## Utility Functions

### Key Functions:

- `getMerkleRoot(const vector<string>& merkle)`: Computes the Merkle root from block data.
- `findHash(int index, string prevHash, vector<string>& merkle)`: Simulates the mining process to find a suitable hash and nonce.
