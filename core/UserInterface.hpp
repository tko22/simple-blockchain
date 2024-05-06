#include <iostream>
#include <vector>
#include <string>
#include "Blockchain.hpp"
#include "Node.hpp"

class UserInterface
{
public:
    UserInterface(Blockchain& blockchain, const std::vector<int>& nodePorts);
    void runCommandLineInterface();

private:
    Blockchain& blockchain;
    const std::vector<int>& nodes;

    void lookAtBlocks();
    void addBlock();
    void sendNewChain(const std::string& json);
};

UserInterface::UserInterface(Blockchain& blockchain,
                             const std::vector<int>& nodePorts)
  : blockchain(blockchain)
  , nodes(nodePorts)
{
}

void UserInterface::runCommandLineInterface()
{
    for (int i = 0; i < 20; ++i) {
        std::cout << "\n(1) Look at Blocks \n(2) Add block\n";
        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1:
                lookAtBlocks();
                break;
            case 2:
                addBlock();
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    }
}

void UserInterface::lookAtBlocks()
{
    std::cout << "What Block do you want to look at? ";
    int blockIndex;
    std::cin >> blockIndex;
    try {
        blockchain.getBlock(blockIndex).toString();
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}

void UserInterface::addBlock()
{
    std::cout << "\nADDING BLOCKS!\nEnter your message: ";
    std::string message;
    std::cin.ignore();
    std::getline(std::cin, message);

    try {
        if (blockchain.getNumOfBlocks() == 0) {
            std::cout << "----------------------------------"
                         "\nPlease join the network... Your "
                         "Blockchain doesn't have any blocks "
                      << std::endl;
            return;
        }

        auto pair = findHash(blockchain.getNumOfBlocks(),
                             blockchain.getLatestBlockHash(),
                             { message });
        blockchain.addBlock(blockchain.getNumOfBlocks(),
                            blockchain.getLatestBlockHash(),
                            pair.first,
                            pair.second,
                            { message });
        sendNewChain(blockchain.toJSON());
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}

void UserInterface::sendNewChain(const std::string& json)
{
    for (int node : nodes) {
        try {
            Node currentNode(node, nodes);
            currentNode.sendNewChain(json);
        } catch (const SimpleWeb::system_error& e) {
            std::cerr << "Client request error: " << e.what() << std::endl;
        }
    }
}
