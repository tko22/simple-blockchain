#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

#include "hash.h"
#include "Block.h"
#include "common.h"
#include "BlockChain.h"

#include "client_ws.hpp"
#include "server_ws.hpp"

using namespace std;

using WsServer = SimpleWeb::SocketServer<SimpleWeb::WS>;
using WsClient = SimpleWeb::SocketClient<SimpleWeb::WS>;

/*
Hash header: index + prevHash + merkleRoot(data) + nonce
*/



int main() {
    printf("Welcome! To quit-> Control c \n");
    WsServer server;
    int port;
    printf("Enter port: ");
    scanf("%d",&port); 
    
    
    
    
    // BLOCK CHAIN INITIALIZATION
    
    char ch;
    printf("Are you the initial Node? (y or n) ");
    scanf(" %c",&ch);
    BlockChain bc;
    if (ch == 'y'){
        bc = BlockChain(0);
    }
    else {
        bc = BlockChain();
        int otherPort;
        int tempInt = -1;
        while ( tempInt == -1 ) {
            printf("Enter ports of nodes in network(no spaces): ");
            if (scanf("%d",&otherPort) == 1) 
                createClient(otherPort,server.config.port);
            else 
                break;
            
        }
    }
    for ( int i = 0; i < 20; i++ ) {
        vector<string> v;
        int temp;
        printf("\n(1) Look at Blocks \n(2) Add block\n");
        scanf("%d",&temp);
        if (temp == 1){
            printf("What Block do you want to look at? ");
            scanf("%d",&temp);
            try {
                bc.getBlock(temp).toString();
            }
            catch (const exception& e){
                cout << e.what() << "\n" << endl;
            }
        }
        else{
            char tmp[201];
            printf("\nADDING BLOCKS!\nEnter your message: ");
            scanf("%200s",tmp);
            string str = tmp;
            printf("Entered '%s' into block\n",str.c_str());
            v.push_back(str);

            
            int in;
            printf("Press any number to add block to blockchain: ");
            scanf("%d",&in);
            try {
                if (bc.getNumOfBlocks() == 0) {
                    printf("----------------------------------\nPlease join the network... Your blockchain doesn't have any blocks ");
                    continue;
                }
                auto pair = findHash(bc.getNumOfBlocks(),bc.getLatestBlockHash(),v);
                bc.addBlock(bc.getNumOfBlocks(),bc.getLatestBlockHash(),pair.first,pair.second,v );
            }
            catch (const exception& e) {
                cout << e.what() << "\n" << endl;
            }
        }
    }

    // bc.addBlock(0,string("00000000000000"),string("003d9dc40cad6b414d45555e4b83045cfde74bcee6b09fb42536ca2500087fd9"),string("46"),v);
    printf("\n");
    return 0;
} 

