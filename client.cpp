#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <arpa/inet.h>

class Client {
public:
    const int numParameters;
    int clientSocket;
    int index;

    Client(int clientIndex, int parameters)
        : numParameters(parameters), clientSocket(-1), index(clientIndex) {
        connectToServer();
        run();
    }

    ~Client() {
        close(clientSocket);
    }

    void sendUpdate(const std::vector<double>& update) {
        send(clientSocket, update.data(), update.size() * sizeof(double), 0);
    }

    void run() {
        try {
            for (int i = 0; i < 10; ++i) {
                // Compute local update based on local data
                std::vector<double> localUpdate(numParameters, 0.1);

                // Send request to the server to get the current model
                std::vector<double> currentModel = getServerModel();

                // Perform some local computation using the current model
                // ...

                // Send the update to the server
                sendUpdate(localUpdate);

                sleep(1);  // Simulate some processing time
            }
        } catch (const std::exception& e) {
            std::cerr << "Exception in run: " << e.what() << std::endl;
        }
    }

    std::vector<double> getServerModel() {
        // Request the current model from the server
        send(clientSocket, &index, sizeof(index), 0);

        // Receive the current model from the server
        std::vector<double> currentModel(numParameters, 0.0);
        ssize_t bytesRead = recv(clientSocket, currentModel.data(), currentModel.size() * sizeof(double), 0);

        if (bytesRead <= 0) {
            perror("Error receiving model from server");
            exit(EXIT_FAILURE);
        }

        return currentModel;
    }

private:

    void connectToServer() {
        clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSocket == -1) {
            perror("Error creating client socket");
            exit(EXIT_FAILURE);
        }


        struct sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(50000);
        serverAddr.sin_addr.s_addr = inet_addr("192.168.200.138");

        if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
            perror("Error connecting to server");
            exit(EXIT_FAILURE);
        }
    }
};

int main() {
    const int numClients = 5;
    const int numParameters = 10;

    for (int i = 0; i < numClients; ++i) {
        Client client(i, numParameters);
    }

    return 0;
}
