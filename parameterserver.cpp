#include <iostream>
#include <vector>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

// Function to aggregate updates from clients
std::vector<double> aggregateUpdates(const std::vector<std::vector<double>>& updates) {
    // averaging for demonstration purposes
    std::vector<double> aggregatedUpdate(updates[0].size(), 0.0);
    for (const auto& update : updates) {
        for (size_t i = 0; i < update.size(); ++i) {
            aggregatedUpdate[i] += update[i];
        }
    }

    for (auto& val : aggregatedUpdate) {
        val /= updates.size();
    }

    return aggregatedUpdate;
}

int main() {
    // Create a socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return -1;
    }

    // Set up server address 
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("192.168.200.138");  
    serverAddr.sin_port = htons(49561);  
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the specified address and port
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error binding socket" << std::endl;
        close(serverSocket);
        return -1;
    }

    // Start listening for incoming connections
    if (listen(serverSocket, 5) == -1) {
        std::cerr << "Error listening for connections" << std::endl;
        close(serverSocket);
        return -1;
    }

    std::cout << "Server is listening for connections on port 49561..." << std::endl;

    // Accept incoming connections
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    // Vector to store updates from clients
    std::vector<std::vector<double>> clientUpdates;

    while (true) {
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if (clientSocket == -1) {
            std::cerr << "Error accepting connection" << std::endl;
            continue;  // Continue to listen for other connections
        }

        std::cout << "Connection accepted from: " << inet_ntoa(clientAddr.sin_addr) << std::endl;

        // Receive update from the client
        std::vector<double> update(10, 0.0);  // Assume 10 parameters 
        recv(clientSocket, update.data(), update.size() * sizeof(double), 0);

        std::cout << "Received update from client: ";
        for (const auto& val : update) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
        close(clientSocket);
    }

    close(serverSocket);

    return 0;
}
