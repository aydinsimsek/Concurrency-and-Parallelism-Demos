#include "server.h"

int TCPServer::acceptClient(sockaddr_in &clientAddr)
{
    socklen_t len = sizeof(clientAddr);
    int newsockfd = accept(sockfd, (struct sockaddr*) &clientAddr, &len); 
    if(newsockfd == -1)
    {
        throw std::runtime_error("ERROR - Accepting client connection failed!");
    }
    return newsockfd;
}

int TCPServer::getServerSocketFd() const
{
    return sockfd;
}

void ClientHandler::handleClient()
{
    char buffer[BUFFER_SIZE];
    while(1)
    {
        memset(buffer, 0, BUFFER_SIZE);
        ssize_t numOfBytes = read(newsockfd, buffer, BUFFER_SIZE);
        if(numOfBytes == -1)
        {
            throw std::runtime_error("ERROR - Reading data from client failed!");
        }
        if(strncmp("Quit.", buffer, 5) == 0)
        {
            std::cout << "Disconnected from " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << std::endl;
            break;
        }
        std::cout << "\nClient " << clientNum << ": " << buffer << std::endl;

        memset(buffer, 0, BUFFER_SIZE); 
        std::cout << "\t\t\t\tServer: ";
        std::cin.getline(buffer, BUFFER_SIZE);  
        numOfBytes = write(newsockfd, buffer, BUFFER_SIZE); 
        if(numOfBytes == -1)
        {
            throw std::runtime_error("ERROR - Writing data to client failed!");
        }
    }
}

int main()
{
    try
    {
        TCPServer server;
        int clientNum = 0;
        while(1)
        {
            sockaddr_in clientAddr{};
            int newsockfd = server.acceptClient(clientAddr);
            clientNum++;
            std::cout << "Client " << clientNum << " connected from " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << std::endl;

            pid_t childProcessID = fork();
            if(childProcessID == 0)
            {
                close(server.getServerSocketFd());
                ClientHandler clientHandler(newsockfd, clientAddr, clientNum);
                clientHandler.handleClient();
                return 0; 
            }
            else if(childProcessID < 0)
            {
                throw std::runtime_error("ERROR - Fork failed!");
            }
            close(newsockfd); 
        }
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return 0;
}