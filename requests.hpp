//author: tko
#ifndef REQUESTS_H
#define REQUESTS_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

#include "json.hh"
using json = nlohmann::json;

using namespace std;

#include "client_http.hpp"

using HttpClient = SimpleWeb::Client<SimpleWeb::HTTP>;


/* 
 * Client request functions 
*/
/*
 * gets the latest chain from the network and finds the longest one
 * used when node initializes 
 * Returns: JSON object of the blockchain
*/ 
json getChainFromNodes(const vector<int> *listOfNodes){
    printf("Pinging nodes for chains....\n");
    vector<string> vect;
    for ( int a = 0; a < (*listOfNodes).size(); a++ ){
        int port = (*listOfNodes)[a]; 
        printf("--- pinging node %d\n",port);
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

/*
 * sends out the new blockchain to network
*/
void sendNewChain(const vector<int> *listOfNodes, string json){
    printf("Sending new chain to network....\n");
    for ( int a = 0; a < (*listOfNodes).size(); a++ ){
        int port = (*listOfNodes)[a]; 
        printf("--- sending to node %d\n",port);
        HttpClient client("localhost:" + to_string(port));
        try {
            auto req = client.request("POST", "/newchain",json);
            cout << "Node " << port << " Response: " << req->content.string() << endl;
        }
        catch(const SimpleWeb::system_error &e) {
            cerr << "Client request error: " << e.what() << endl;
        }
    }
}
/*
 * adds self to network
 * called when node initializes - used so nodes could keep track of nodes in the network
*/
void addSelfToNetwork(const vector<int> *listOfNodes,int port) {
    printf("Sending port to all nodes\n");
    json j;
    j["port"] = port;
    for ( int a = 0; a < (*listOfNodes).size(); a++ ){
        int port = (*listOfNodes)[a]; 
        printf("--- sending port to node %d\n",port);
        HttpClient client("localhost:"+to_string(port));
        try {
            auto req = client.request("POST","/addnode",j.dump(3));
            cout << "Node " << port << " Reponse: " << req->content.string() << endl;
        }
        catch(const SimpleWeb::system_error &e) {
            cerr << "Client request error: " << e.what() << endl;
        }
    }
}




#endif