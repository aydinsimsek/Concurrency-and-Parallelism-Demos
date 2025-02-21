#ifndef SERVER_H_
#define SERVER_H_

#include <iostream>
#include <cstring>
#include <stdexcept>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024  
#define PORT_NUM 5001 
#define BACKLOG 5 

class TCPServer
{
    private:
        int sockfd;

    public: 
        TCPServer()
        {
            sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if(sockfd == -1)
            {
                throw std::runtime_error("ERROR - Creating the server socket failed!");
            }

            sockaddr_in serverAddr{};
            serverAddr.sin_family = AF_INET;
            serverAddr.sin_addr.s_addr = INADDR_ANY;
            serverAddr.sin_port = htons(PORT_NUM);

            if(bind(sockfd, (struct sockaddr*) &serverAddr, sizeof(serverAddr)) == -1)
            {
                throw std::runtime_error("ERROR - Binding the socket failed!");
            }

            if(listen(sockfd, BACKLOG) == -1)
            {
                throw std::runtime_error("ERROR - Listening for connections failed!");
            }

            std::cout << "Server listening for connections..." << std::endl;
        }
        ~TCPServer()
        {
            close(sockfd);
        }
		int acceptClient(sockaddr_in &clientAddr);
		int getServerSocketFd() const;
};

class ClientHandler
{
    private:
        int newsockfd;
        sockaddr_in clientAddr;
        int clientNum;

    public:
        ClientHandler(int fd, sockaddr_in addr, int num) : newsockfd(fd), clientAddr(addr), clientNum(num) {}
        ~ClientHandler()
        {
            close(newsockfd);
        }
		void handleClient();
};

#endif
