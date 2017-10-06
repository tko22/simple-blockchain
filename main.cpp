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

/*
Hash header: index + prevHash + merkleRoot(data) + nonce
*/

int main() {
    printf("Welcome! To quit-> Control c \n");
    WsServer server;
    server.config.port = 8000;

    auto &echo = server.endpoint["^/echo/?$"];

    echo.on_message = [](shared_ptr<WsServer::Connection> connection, shared_ptr<WsServer::Message> message) {
        auto message_str = message->string();

        cout << "Server: Message received: \"" << message_str << "\" from " << connection.get() << endl;

        cout << "Server: Sending message \"" << message_str << "\" to " << connection.get() << endl;

        auto send_stream = make_shared<WsServer::SendStream>();
        *send_stream << message_str;
        // connection->send is an asynchronous function
        connection->send(send_stream, [](const SimpleWeb::error_code &ec) {
        if(ec) {
            cout << "Server: Error sending message. " <<
                // See http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/reference.html, Error Codes for error code meanings
                "Error: " << ec << ", error message: " << ec.message() << endl;
        }
        });

        // Alternatively, using a convenience function:
        // connection->send(message_str, [](const SimpleWeb::error_code & /*ec*/) { /*handle error*/ });
    };

    echo.on_open = [](shared_ptr<WsServer::Connection> connection) {
        cout << "Server: Opened connection " << connection.get() << endl;
    };

    // See RFC 6455 7.4.1. for status codes
    echo.on_close = [](shared_ptr<WsServer::Connection> connection, int status, const string & /*reason*/) {
        cout << "Server: Closed connection " << connection.get() << " with status code " << status << endl;
    };

    // See http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/reference.html, Error Codes for error code meanings
    echo.on_error = [](shared_ptr<WsServer::Connection> connection, const SimpleWeb::error_code &ec) {
        cout << "Server: Error in connection " << connection.get() << ". "
            << "Error: " << ec << ", error message: " << ec.message() << endl;
    };
    thread server_thread([&server]() {
        // Start WS-server
        server.start();
      });
      printf("\nWebsocket starting on Port 8000" );
      
    char tmp[201];
    int ch;
    auto bc = BlockChain();
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
                cout << e.what() << "\n";
            }
        }
        else{
            printf("\nADDING BLOCKS!\nEnter your message: ");
            scanf("%200s",tmp);
            string str = tmp;
            printf("Entered '%s' into block\n",str.c_str());
            v.push_back(str);
        
            printf("Press any number to add block to blockchain: ");
            scanf("%d",&ch);
        
            auto pair = findHash(bc.getNumOfBlocks(),bc.getLatestBlockHash(),v);
            bc.addBlock(bc.getNumOfBlocks(),bc.getLatestBlockHash(),pair.first,pair.second,v );
        }
    }
        
    

    // bc.addBlock(0,string("00000000000000"),string("003d9dc40cad6b414d45555e4b83045cfde74bcee6b09fb42536ca2500087fd9"),string("46"),v);
    printf("\n");
    return 0;
} 

