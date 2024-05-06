#pragma once

#include <iostream>
#include <functional>
#include "../network/server_http.hpp"

using HttpServer = SimpleWeb::Server<SimpleWeb::HTTP>;

class Server
{
public:
    Server(int port);
    void start();
    int getPort();
    void addResource(
      const std::string& resourcePattern,
      const std::string& method,
      std::function<void(std::shared_ptr<HttpServer::Response>,
                         std::shared_ptr<HttpServer::Request>)> handler);
    void onError(
      std::function<void(std::shared_ptr<HttpServer::Request>,
                         const SimpleWeb::error_code& ec)> errorHandler);

private:
    HttpServer server;
    int port;
    std::thread server_thread;
};

Server::Server(int port)
  : port(port)
{
}

void Server::start()
{
    std::cout << "Starting server at port " << port << std::endl;
    server.config.port = port;
    server_thread = std::thread([this]() { server.start(); });
}

int Server::getPort()
{
    return port;
}

void Server::addResource(
  const std::string& resourcePattern,
  const std::string& method,
  std::function<void(std::shared_ptr<HttpServer::Response>,
                     std::shared_ptr<HttpServer::Request>)> handler)
{
    server.resource[resourcePattern][method] = handler;
}

void Server::onError(
  std::function<void(std::shared_ptr<HttpServer::Request>,
                     const SimpleWeb::error_code& ec)> errorHandler)
{
    server.on_error = errorHandler;
}