#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

using namespace std;

int main()
{
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        cerr << "Error creating socket!" << endl;
        return 1;
    }
    cout << "Client socket created successfully" << endl;

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");  

    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        cerr << "Connection failed!" << endl;
        close(clientSocket);
        return 1;
    }
    cout << "Connected to server successfully!" << endl;


    const char* message = "Hello, server!";
    send(clientSocket, message, strlen(message), 0);
    cout << "Message sent: " << message << endl;

    close(clientSocket);
    cout << "Connection closed" << endl;

    return 0;
}