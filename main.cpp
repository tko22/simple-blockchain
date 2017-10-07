#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

#include "hash.h"
#include "Block.h"
#include "common.h"
#include "BlockChain.h"

#include "json.hh"
using json = nlohmann::json;

using namespace std;

#include "client_http.hpp"
#include "server_http.hpp"

using HttpServer = SimpleWeb::Server<SimpleWeb::HTTP>;
using HttpClient = SimpleWeb::Client<SimpleWeb::HTTP>;

/*
Hash header: index + prevHash + merkleRoot(data) + nonce
*/
json getChainFromNodes(const vector<int> *listOfNodes){
    printf("Pinging nodes for chains....\n");
    vector<string> vect;
    for ( int a = 0; a < (*listOfNodes).size(); a++ ){
        int port = (*listOfNodes)[a]; 
        HttpClient client("localhost:"+to_string(port));
        try {
            auto req = client.request("GET", "/latestchain");
            vect.push_back(req->content.string());
        }
        catch(const SimpleWeb::system_error &e) {
            cerr << "Client request error: " << e.what() << endl;
        }
    }

    //find biggest blockchain
    json biggest_bc = json::parse(vect[0]);
    int max = 0;
    for (int i =0; i < vect.size(); i++) {
        auto json_data = json::parse(vect[i]);
        if ( max < json_data["length"].get<int>() ){
            max = json_data["length"].get<int>();
            biggest_bc = json_data;
        }
    }
    return biggest_bc;
    
}

void sendNewChain(const vector<int> *listOfNodes, string json){
    printf("Sending new chain to network....\n");
    for ( int a = 0; a < (*listOfNodes).size(); a++ ){
        int port = (*listOfNodes)[a]; 
        printf("--- sending to node %d",port);
        HttpClient client("localhost:"+to_string(port));
        try {
            auto req = client.request("POST", "/newchain",json);
            cout << "Node " << port << " Reponse: " << req->content.string() << endl;
        }
        catch(const SimpleWeb::system_error &e) {
            cerr << "Client request error: " << e.what() << endl;
        }
    }
}

int main() {
    printf("Welcome! To quit-> Control c \n");
    HttpServer server;

    int port;
    printf("Enter port: ");
    scanf("%d",&port); 
    server.config.port = port; //server port
    
    vector<int> listOfNodes; //vector of ports of nodes
    
    // BLOCK CHAIN INITIALIZATION
    
    char ch;
    printf("Are you the initial Node? (y or n) ");
    scanf(" %c",&ch);
    BlockChain bc;
    if (ch == 'y'){
        bc = BlockChain(0);
    }
    else if(ch =='n'){
        bc = BlockChain(0);
        char otherPorts[50];
        printf("Enter ports of nodes in network(with commas in between): ");
        scanf("%s",otherPorts);
        stringstream ss(otherPorts);
        int i;
            while (ss >> i)
            {
                listOfNodes.push_back(i);
                if (ss.peek() == ',' || ss.peek() == ' ')
                    ss.ignore();
            }
        json chain = getChainFromNodes(&listOfNodes);
        //skips first block - same genesis block across all nodes
        for (int a = 1; a <chain["length"].get<int>(); a++ ){
            auto block = chain["data"][a];
            vector<string> data = block["data"].get<vector<string> >();
            bc.addBlock(block["index"],block["previousHash"],block["hash"],block["nonce"],data);
        } 
    }
    else {
        return 0;
    }

    // SERVER INITIALIZATION

    server.resource["^/latestchain$"]["GET"] = [&bc](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
        printf("GET /latestchain --- Sending BlockChain....\n");
        response->write(bc.toJSON());
    };
    server.resource["^/newchain$"]["POST"] = [&bc](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
        try {
            cout << "POST /newchain ...\n";
            json content = json::parse(request->content);
            if (content["length"].get<int>() > bc.getNumOfBlocks()){
                cout << "response length" << content["length"].get<int>();
                cout << "bc length" << bc.getNumOfBlocks();
                bc.replaceChain(content);
                response->write("Replaced Chain");
            }
            response->write("Same Chain Size -- invalid");
        }
        catch(const exception &e) {
            *response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << strlen(e.what()) << "\r\n\r\n" << e.what();
        }
    };

    server.on_error = [](shared_ptr<HttpServer::Request> /*request*/, const SimpleWeb::error_code & ec) {
        cout << "SERVER ERROR: " << ec << endl;
      };
    printf("Starting server at %d",server.config.port);

    thread server_thread([&server]() {
        // Start server
        server.start();
    });
    
    //COMMAND LINE INTERFACE

    for ( int i = 0; i < 20; i++ ) {
        vector<string> v;
        int temp;
        printf("\n(1) Look at Blocks \n(2) Add block\n");
        int valid = scanf("%d",&temp);
        if ( (valid == 1) && (temp == 1)){
            printf("What Block do you want to look at? ");
            scanf("%d",&temp);
            try {
                bc.getBlock(temp).toString();
            }
            catch (const exception& e){
                cout << e.what() << "\n" << endl;
            }
        }
        else if (temp == 2){
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
                sendNewChain(&listOfNodes,bc.toJSON());
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

