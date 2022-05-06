// CPSC 441 - Assignment 2 - Currency Converter Service
// made by Kaumil Patel 30088096

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <thread>
#include <algorithm>

#include "../utility/rdt.h"

// Currency Converter Service
#define currencyPort 12347

using namespace std;

int bufferSize = 512;
int parentsockfd;

float CADtoUSD(float &amount) {
    return amount * 0.80f;
}

float CADtoEUR(float &amount) {
    return amount * 0.69f;
}

float CADtoGBP(float &amount) {
    return amount * 0.59f;
}

float CADtoBTC(float &amount) {
    return amount * 0.000016f;
}

void handleConnection() {
    struct sockaddr_in caddr;
    char incomingBuffer[bufferSize];
    char outgoingBuffer[bufferSize];
    char *body;
    int len;
    bool seqNum = false;

    // receives data from main server
    recvRDT(parentsockfd, caddr, outgoingBuffer, incomingBuffer, bufferSize, len, body);
    if (len < 0) {
        cout << "Error: " << errno << endl;
        return;
    }
    cout << "Connected to IP: " << inet_ntoa(caddr.sin_addr) << " - Port: " << ntohs(caddr.sin_port) << endl;

    // user request is processed
    string::size_type i;
    float amount = stof(body, &i);
    i++;
    if (strncasecmp(&body[i], "CAD USD", 7) == 0) {
        sprintf(outgoingBuffer, "%.2f in Canadian Dollar is %.2f in American Dollar", amount, CADtoUSD(amount));
    } else if (strncasecmp(&body[i], "CAD EUR", 7) == 0) {
        sprintf(outgoingBuffer, "%.2f in Canadian Dollar is %.2f in Euro", amount, CADtoEUR(amount));
    } else if (strncasecmp(&body[i], "CAD GBP", 7) == 0) {
        sprintf(outgoingBuffer, "%.2f in Canadian Dollar is %.2f in Pound sterling", amount, CADtoGBP(amount));
    } else if (strncasecmp(&body[i], "CAD BTC", 7) == 0) {
        sprintf(outgoingBuffer, "%.2f in Canadian Dollar is %.8f in Bitcoin", amount, CADtoBTC(amount));
    } else {
        strcpy(outgoingBuffer, "Invalid option");
    }

    // results are sent back to main server
    sendRDT(parentsockfd, caddr, seqNum, outgoingBuffer, incomingBuffer, bufferSize, len);
    if (len < 0) {
        return;
    }
}

void exiting() {
    close(parentsockfd);
    cout << "Exiting" << endl;
    exit(0);
}

int main() {
    // clean up before program exits
    atexit(exiting);

    struct sockaddr_in saddr;

    // Another socket is created, which can be used to connect users to the server.
    // socket is set to use ipv4 and udp
    if ((parentsockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        cout << "Error creating socket: " << errno << endl;
        close(parentsockfd);
        exit(1);
    }

    // set 3 second timeout
    struct timeval tv;
    tv.tv_sec = 3;
    tv.tv_usec = 0;
    setsockopt(parentsockfd, SOL_SOCKET, SO_RCVTIMEO, (const char *) &tv, sizeof tv);

    memset(&saddr, 0, sockSize);
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(currencyPort);

    // binds main socket to all network interfaces and main port
    if ((bind(parentsockfd, (struct sockaddr *) &saddr, sockSize)) < 0) {
        cout << "Error binding: " << errno << endl;
        close(parentsockfd);
        exit(1);
    }
    cout << "Server is ready on port " << currencyPort << endl << endl;

    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(parentsockfd, &rfds);
    while (true) {
        // wait for incoming connection from main server
        int recVal = select(parentsockfd + 1, &rfds, nullptr, nullptr, nullptr);
        if (recVal < 0) {
            cout << "Error selecting: " << errno << endl;
        } else {
            // process request from server
            thread child(handleConnection);
            child.detach();
        }
    }
}
