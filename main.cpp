#include <iostream>
#include "Node.hpp"
#include "UserInterface.hpp"

int main(int argc, char* argv[])
{
    try {
        // Check command-line arguments for the node port
        if (argc < 2) {
            std::cerr << "Usage: " << argv[0] << " <port>"
                      << " <node ports>" << std::endl;
            return 1;
        }

        int port = std::stoi(argv[1]);
        std::vector<int> nodePorts;

        for (int i = 2; i < argc; i++) {
            nodePorts.push_back(std::stoi(argv[i]));
        }

        // Initialize the blockchain node
        Node node(port, nodePorts);
        node.init();

        // Attempt to add this node to an existing network
        node.addSelfToNetwork();

        // Initialize user interface and bind it to the blockchain and node list
        UserInterface ui(node.getBlockchain(), node.getListOfNodes());

        // Run the command-line user interface
        ui.runCommandLineInterface();

        // Normally, a mechanism to safely shut down the server should be in
        // place For demo or development purposes, you might simply stop the
        // server manually or let it run indefinitely
    } catch (const std::exception& e) {
        std::cerr << "Exception caught in main: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}
