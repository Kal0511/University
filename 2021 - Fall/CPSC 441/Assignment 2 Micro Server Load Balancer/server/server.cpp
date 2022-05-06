// CPSC 441 - Assignment 2 - Indirection Server
// made by Kaumil Patel 30088096

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <thread>

#include "../utility/rdt.h"


#define primaryPort 12345

// Translator Service
#define translatorPort 12346
// Currency Converter Service
#define currencyPort 12347
// Voting Service
#define votingPort 12348

using namespace std;

// server addresses for micro services
char currencyAddress[] = "127.0.0.1";
char votingAddress[] = "127.0.0.1";
char translatorAddress[] = "127.0.0.1";

int bufferSize = 512;
int parentsockfd;

// handles incoming client connections
void handleConnection(int clientsockfd) {
    int len;
    char incomingBuffer[bufferSize];
    char outgoingBuffer[bufferSize];
    char *body;
    bool seqNum = false;
    struct sockaddr_in addr;
    memset(&addr, 0, sockSize);
    addr.sin_family = AF_INET;
    bool getKey;
    int servicesockfd;

    // Socket is created, which can be used to talk to the micro servers.
    // socket is set to use ipv4 and udp
    if ((servicesockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        cout << "Error creating socket: " << errno << endl;
        close(servicesockfd);
        close(clientsockfd);
        return;
    }

    // 3 second time is set for microservice socket
    struct timeval tv;
    tv.tv_sec = 3;
    tv.tv_usec = 0;
    setsockopt(servicesockfd, SOL_SOCKET, SO_RCVTIMEO, (const char *) &tv, sizeof tv);

    // enter main loop
    // runs until user closes connection
    while (true) {
        getKey = true;

        // receives command from user
        if ((len = recv(clientsockfd, incomingBuffer, bufferSize, 0)) < 0) {
            close(servicesockfd);
            close(clientsockfd);
            return;
        }
        incomingBuffer[len] = '\0';
        cout << "TCP Received: " << incomingBuffer << endl;

        // process user request
        switch (incomingBuffer[0]) {
            case '1':
                addr.sin_addr.s_addr = inet_addr(translatorAddress);
                addr.sin_port = htons(translatorPort);
                strcpy(outgoingBuffer, "enter english word");
                break;
            case '2':
                addr.sin_addr.s_addr = inet_addr(currencyAddress);
                addr.sin_port = htons(currencyPort);
                strcpy(outgoingBuffer, "$[amount] CAD [currency]");
                break;
            case '3':
                addr.sin_addr.s_addr = inet_addr(votingAddress);
                addr.sin_port = htons(votingPort);
                // gets encryption key based on input
                if (incomingBuffer[1] == '1') {
                    strcpy(outgoingBuffer, "getList");
                    getKey = false;
                } else {
                    strcpy(outgoingBuffer, "getKey");
                    sendRDT(servicesockfd, addr, seqNum, outgoingBuffer, incomingBuffer, bufferSize, len);
                    if (len < 0) {
                        cout << "Error: " << errno << endl;
                        strcpy(outgoingBuffer, "Micro service is offline");
                        if (send(clientsockfd, outgoingBuffer, strlen(outgoingBuffer), 0) < 0) {
                            close(servicesockfd);
                            close(clientsockfd);
                        }
                        return;
                    }
                    recvRDT(servicesockfd, addr, outgoingBuffer, incomingBuffer, bufferSize, len, body);
                    if (len < 0) {
                        cout << "Error: " << errno << endl;
                        strcpy(outgoingBuffer, "Micro service is offline");
                        if (send(clientsockfd, outgoingBuffer, strlen(outgoingBuffer), 0) < 0) {
                            close(servicesockfd);
                            close(clientsockfd);
                        }
                        return;
                    }
                    strcpy(outgoingBuffer, body);
                }
                break;
            case '4':
                strcpy(outgoingBuffer, "Connection Closed");
                if (send(clientsockfd, outgoingBuffer, strlen(outgoingBuffer), 0) < 0) {
                    close(servicesockfd);
                    close(clientsockfd);
                    return;
                }
                close(servicesockfd);
                close(clientsockfd);
                return;
            default:
                strcpy(outgoingBuffer, "Invalid input");
                if (send(clientsockfd, outgoingBuffer, strlen(outgoingBuffer), 0) < 0) {
                    close(servicesockfd);
                    close(clientsockfd);
                    return;
                }
                continue;
        }
        if (getKey) {
            // sends instructions back to client
            if ((send(clientsockfd, outgoingBuffer, strlen(outgoingBuffer), 0)) < 0) {
                close(servicesockfd);
                close(clientsockfd);
                return;
            }
            cout << "TCP Sent: " << outgoingBuffer << endl;

            // receives user request
            if ((len = recv(clientsockfd, incomingBuffer, bufferSize, 0)) < 0) {
                close(servicesockfd);
                close(clientsockfd);
                return;
            }
            incomingBuffer[len] = '\0';
            cout << "TCP Received: " << incomingBuffer << endl;
            strcpy(outgoingBuffer, incomingBuffer);
        }

        // sends user request to microservice
        sendRDT(servicesockfd, addr, seqNum, outgoingBuffer, incomingBuffer, bufferSize, len);
        if (len < 0) {
            cout << "Error: " << errno << endl;
            strcpy(outgoingBuffer, "Micro service is offline");
            if (send(clientsockfd, outgoingBuffer, strlen(outgoingBuffer), 0) < 0) {
                close(servicesockfd);
                close(clientsockfd);
            }
            return;
        }

        // receives data from micro service
        recvRDT(servicesockfd, addr, outgoingBuffer, incomingBuffer, bufferSize, len, body);
        if (len < 0) {
            cout << "Error: " << errno << endl;
            strcpy(outgoingBuffer, "Micro service is offline");
            if (send(clientsockfd, outgoingBuffer, strlen(outgoingBuffer), 0) < 0) {
                close(servicesockfd);
                close(clientsockfd);
            }
            return;
        }

        // sends the results to user
        if (send(clientsockfd, body, strlen(body), 0) < 0) {
            close(servicesockfd);
            close(clientsockfd);
            return;
        }
        cout << "TCP Sent: " << body << endl;
    }
}

void exiting() {
    close(parentsockfd);
    cout << "Exiting" << endl;
    exit(0);
}

int main() {
    // closes socket before exiting program
    atexit(exiting);

    struct sockaddr_in saddr, caddr;


    // Socket is created, which can be used to connect users to the server.
    // socket is set to use ipv4 and tcp
    if ((parentsockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "Error creating socket: " << errno << endl;
        close(parentsockfd);
        exit(1);
    }

    memset(&saddr, 0, sockSize);
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(primaryPort);

    // binds main socket to all network interfaces and main port
    if ((bind(parentsockfd, (struct sockaddr *) &saddr, sockSize)) < 0) {
        cout << "Error binding: " << errno << endl;
        exit(1);
    }

    // sets parent socket to accept client connection requests and have listening queue of 16
    if (listen(parentsockfd, 16) < 0) {
        cout << "Error listening: " << errno << endl;
        exit(1);
    }
    cout << "Server is listening on port " << primaryPort << endl << endl;


    // enters main loop which accepts incoming connections to the server
    while (true) {
        int childsockfd;
        // accepts the first client in the queue and creates a new file descriptor for that connection
        if ((childsockfd = accept(parentsockfd, (struct sockaddr *) &caddr, (socklen_t *) &sockSize)) < 0) {
            close(childsockfd);
            if (errno == 22) {
                cout << "Server has stopped accepting new connections" << endl;
                exit(0);
            }
            cout << "Error accepting: " << errno << endl;
            cout << "IP: " << inet_ntoa(caddr.sin_addr) << " - Port: " << ntohs(caddr.sin_port) << endl;
            exit(1);
        }

        cout << "Accepted connection from IP: " << inet_ntoa(caddr.sin_addr) << " - Port: " << ntohs(caddr.sin_port) << endl;
        thread child(handleConnection, childsockfd);
        child.detach();
    }
}
