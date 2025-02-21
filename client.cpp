#include "client.h"

void TCPClient::connectToServer()
{
    if(connect(sockfd, (struct sockaddr*) &serverAddr, sizeof(serverAddr)) == -1)
    {
        throw std::runtime_error("ERROR - Connecting to the server failed!");
    }
    std::cout << "Connected to the server (Type Quit. to disconnect)" << std::endl;
}

int TCPClient::getClientSocketFd() const
{
    return sockfd;
}

void Communication::sendMessage(int fd, const char *buffer)
{
    ssize_t numOfBytes = write(fd, buffer, BUFFER_SIZE);
    if(numOfBytes == -1)
    {
        throw std::runtime_error("ERROR - Writing data to server failed!");
    }
}

void Communication::receiveMessage(int fd, char *buffer)
{
    ssize_t numOfBytes = read(fd, buffer, BUFFER_SIZE);
    if(numOfBytes == -1)
    {
        throw std::runtime_error("ERROR - Reading data from server failed!");
    }
    std::cout << "Server: " << buffer << std::endl;
}

int main()
{
    try
    {
        TCPClient client("127.0.0.1", PORT_NUM); // using localhost for testing 
        client.connectToServer();
        Communication comm;
        char buffer[BUFFER_SIZE];
        while(1)
        {
            memset(buffer, 0, BUFFER_SIZE); 
            std::cout << "\t\t\t\tMe: ";
            std::cin.getline(buffer, BUFFER_SIZE);  
            comm.sendMessage(client.getClientSocketFd(), buffer);
            if(strncmp("Quit.", buffer, 5) == 0)
            {
                std::cout << "Disconnected from the server." << std::endl;
                break;
            }

            memset(buffer, 0, BUFFER_SIZE);
            comm.receiveMessage(client.getClientSocketFd(), buffer);
        }
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return 0;
}