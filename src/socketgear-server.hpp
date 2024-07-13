#pragma once
#include <string>
#include <unordered_map>
#include <vector>

namespace socketGearServer{
    class Server{
        typedef struct {
            std::vector<int> sock;
            int eventsType;
        }Channel;

        typedef struct {
            int sockfd;
            std::vector<Channel> channels;
        }Session;

        std::unordered_map<std::string, Session> sessions;
        std::unordered_map<std::string, Channel> channels;

        int serverSocket;
        std::vector<int> clientSockets;
        std::mutex clientSocketsMutex;

        public:
        Server(int port=26900);

        // Channel handlers
        char create_channel();
        char delete_channel(std::string uuid);
        char subscribe_channel(std::string uuid);
        char unsubscribe_channel(std::string uuid);

        // Event
        // TODO meta event instead of real events
        private:
        void onConnection(int clientSocket);
        char onEvent();
    };
}