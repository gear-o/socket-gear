#include "socketgear-client.hpp"
#include <cassert>
#include "utils.hpp"
#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

namespace socketGearClient{
    Client::Client(const std::string& serverIp="", const int serverPort=-1, std::string uuid=""){
        if(serverIp == ""){
            // Unvalidate client
            assert("server is not set");
        }
        if(uuid == ""){
            this->session.uuid = socketGearUtils::uuid();
        }
        // TODO make uuid persistent

        int sockfd;
        struct sockaddr_in serverAddr;
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            std::cerr << "Error creating socket!" << std::endl;
            return;
        }

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(serverPort);
        serverAddr.sin_addr.s_addr = inet_addr(serverIp.c_str());
        memset(&(serverAddr.sin_zero), '\0', 8);

        if (connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(struct sockaddr)) < 0) {
            assert("Error connecting to server");
        }
    }

    char Client::sendEvent(std::string toUuid, std::string event, const char* message){
        if(this->session.sockfd){
            if (send(this->session.sockfd, message, strlen(message), 0) < 0) {
                std::cerr << "Error sending message!" << std::endl;
            }
            char buffer[1024];
            ssize_t bytesReceived = recv(this->session.sockfd, buffer, sizeof(buffer) - 1, 0);
            if (bytesReceived < 0) {
                std::cerr << "Error receiving message!" << std::endl;
            } else {
                buffer[bytesReceived] = '\0'; // Null-terminate the received data
                std::cout << "Received: " << buffer << std::endl;
            }
        }
    }
}