#pragma once

#include <vector>
#include <string>

#include "Blockchain.hpp"
#include "Server.hpp"
#include "../network/client_http.hpp"
#include "../utilities/json.hh"

using json = nlohmann::json;
using HttpClient = SimpleWeb::Client<SimpleWeb::HTTP>;

class Node
{
public:
    Node(int port, std::vector<int> nodes);
    void init();

    void addSelfToNetwork();
    json getChainFromNodes();
    void sendNewChain(const std::string& json);
    Blockchain& getBlockchain();
    std::vector<int>& getListOfNodes();

private:
    Blockchain bc;
    Server nodeServer;
    std::vector<int> listOfNodes;
};

Node::Node(int port, std::vector<int> nodes)
  : bc(false)
  , nodeServer(port)
  , listOfNodes(nodes)
{
}

void Node::init()
{
    // Add routes to handle HTTP requests
    nodeServer.addResource(
      "/addnode",
      "POST",
      [this](std::shared_ptr<HttpServer::Response> response,
             std::shared_ptr<HttpServer::Request> request) {
          printf("POST /addnode --- New Node adding to network....\n");
          try {
              json content = json::parse(request->content);
              int port = content["port"].get<int>();
              listOfNodes.push_back(port); // Adds port to listOfNodes
              printf("----Adding node %d to listOfNodes\n", port);
              response->write("Added You to our List");
          } catch (const std::exception& e) {
              *response << "HTTP/1.1 400 Bad Request\r\nContent-Length: "
                        << strlen(e.what()) << "\r\n\r\n"
                        << e.what();
          }
      });

    nodeServer.addResource(
      "/latestchain",
      "GET",
      [this](std::shared_ptr<HttpServer::Response> response,
             std::shared_ptr<HttpServer::Request> request) {
          printf("GET /latestchain --- Sending Blockchain....\n");
          response->write(bc.toJSON());
          printf("---Sent current Blockchain\n");
      });

    nodeServer.addResource(
      "/newchain",
      "POST",
      [this](std::shared_ptr<HttpServer::Response> response,
             std::shared_ptr<HttpServer::Request> request) {
          std::cout << "POST /newchain --- Node in Network sent new chain\n";
          try {
              json content = json::parse(request->content);
              if (content["length"].get<std::size_t>() > bc.getNumOfBlocks()) {
                  bc.replaceChain(content);
                  std::cout << "----Replaced current chain with new one"
                            << std::endl;
                  response->write("Replaced Chain\n");
              } else {
                  std::cout
                    << "----Chain was not replaced: sent chain had same size"
                    << std::endl;
                  response->write("Same Chain Size -- invalid");
              }
          } catch (const std::exception& e) {
              *response << "HTTP/1.1 400 Bad Request\r\nContent-Length: "
                        << strlen(e.what()) << "\r\n\r\n"
                        << e.what();
          }
      });

    // Set error handler
    nodeServer.onError([](std::shared_ptr<HttpServer::Request> /*request*/,
                          const SimpleWeb::error_code& ec) {
        if (ec.message() != "End of file") {
            std::cout << "SERVER ERROR: " << ec.message() << std::endl;
        }
    });

    // Start the server
    nodeServer.start();
}

void Node::addSelfToNetwork()
{
    json j;
    j["port"] = nodeServer.getPort();
    for (int a : listOfNodes) {
        try {
            HttpClient client("localhost:" + std::to_string(a));
            auto req = client.request("POST", "/addnode", j.dump());
            std::cout << "Node " << a << " Response: " << req->content.string()
                      << std::endl;
        } catch (const SimpleWeb::system_error& e) {
            std::cerr << "Client request error: " << e.what() << std::endl;
        }
    }
}

json Node::getChainFromNodes()
{
    std::vector<std::string> vect;
    for (int a : listOfNodes) {
        try {
            HttpClient client("localhost:" + std::to_string(a));
            auto req = client.request("GET", "/latestchain");
            vect.push_back(req->content.string());
        } catch (const SimpleWeb::system_error& e) {
            std::cerr << "Client request error: " << e.what() << std::endl;
        }
    }

    json biggest_bc = json::parse(vect[0]);
    int max = 0;
    for (std::size_t i = 0; i < vect.size(); i++) {
        auto json_data = json::parse(vect[i]);
        if (max < json_data["length"].get<int>()) {
            max = json_data["length"].get<int>();
            biggest_bc = json_data;
        }
    }
    return biggest_bc;
}

void Node::sendNewChain(const std::string& json)
{
    for (int a : listOfNodes) {
        try {
            HttpClient client("localhost:" + std::to_string(a));
            auto req = client.request("POST", "/newchain", json);
            std::cout << "Node " << a << " Response: " << req->content.string()
                      << std::endl;
        } catch (const SimpleWeb::system_error& e) {
            std::cerr << "Client request error: " << e.what() << std::endl;
        }
    }
}

Blockchain& Node::getBlockchain()
{
    return bc;
}

std::vector<int>& Node::getListOfNodes()
{
    return listOfNodes;
}