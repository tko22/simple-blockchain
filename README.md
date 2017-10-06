# simple-blockchain
My implementation of a blockchain in C++ I created for fun :)
Follows some Bitcoin design choices including using SHA-256 to hash headers and blocks, merkle trees, and mining WoS. 

### BlockChain Class
Every time you want to add a block to the blockchain, you will need to provide it: \n
`int index, string prevHash, string hash, string nonce, vector<string> &merkle`
index - index of the block
prevHash - hash of the previous block
nonce - self-explantory
merkle - vector holding in the data of the block

### Mining
(I made it very simple because I didn't want to spend much processing power)
first two characters of the hash must be 0
e.g. `003d9dc40cad6b414d45555e4b83045cfde74bcee6b09fb42536ca2500087fd9` works 


### Block 
Hash header: index + prevHash + merkleRoot(data) + nonce
Data: vector of strings


Peer-to-Peer coming soon!
