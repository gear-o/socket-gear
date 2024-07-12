#pragma once
#include <string>
#include <uuid/uuid.h>

namespace socketGearClient{
    class Client{
        typedef struct {
            std::string uuid;
            int sockfd;
        }Session;

        private:
        Session session;

        public:
        Client(const std::string& serverIp="", const int serverPort=-1, std::string uuid="");

        char joinChannel(std::string uuid);

        char sendEvent(std::string toUuid, std::string event, const char* message);
        char receiveEvent();
    };
}