#include "socketgear-server.hpp"
#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <vector>
#include <thread>
#include <mutex>
#include <cassert>

namespace socketGearServer{
    Server::Server(int port=26900){
        // TODO await for connection
        // TODO store connection into container as session
        // Create socket
        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket < 0) {
            assert("Error creating socket!");
        }

        // Bind socket to the specified port
        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(port);
        memset(&(serverAddr.sin_zero), '\0', 8);

        if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            std::cerr << "Error binding socket!" << std::endl;
            return;
        }

        // Listen for incoming connections
        if (listen(serverSocket, 5) < 0) {
            std::cerr << "Error listening on socket!" << std::endl;
            return;
        }

        std::cout << "Server listening on port " << port << std::endl;

        // Accept connections in a loop
        while (true) {
            sockaddr_in clientAddr;
            socklen_t clientAddrSize = sizeof(clientAddr);
            int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);
            if (clientSocket < 0) {
                std::cerr << "Error accepting connection!" << std::endl;
                continue;
            }

            std::cout << "Accepted a connection!" << std::endl;

            // Store the client socket
            {
                std::lock_guard<std::mutex> lock(clientSocketsMutex);
                clientSockets.push_back(clientSocket);
            }

            // Handle the connection in a separate thread
            std::thread(&Server::handleClient, this, clientSocket).detach();
        }
    }

    void Server::handleClient(int clientSocket) {
        char buffer[1024];
        while (true) {
            ssize_t bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
            if (bytesReceived <= 0) {
                break; // Connection closed or error occurred
            }

            buffer[bytesReceived] = '\0';
            std::cout << "Received: " << buffer << std::endl;

            // Echo the message back to the client
            send(clientSocket, buffer, bytesReceived, 0);
        }

        close(clientSocket);
        std::lock_guard<std::mutex> lock(clientSocketsMutex);
        clientSockets.erase(std::remove(clientSockets.begin(), clientSockets.end(), clientSocket), clientSockets.end());
        std::cout << "Connection closed!" << std::endl;
    }

}