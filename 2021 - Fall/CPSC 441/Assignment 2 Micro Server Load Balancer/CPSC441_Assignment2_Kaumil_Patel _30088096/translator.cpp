// CPSC 441 - Assignment 2 - Translator Service
// made by Kaumil Patel 30088096

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <thread>
#include <algorithm>
#include <fstream>

#include "utility/rdt.h"

// Translator Service
#define translatorPort 12346

using namespace std;

struct word {
    string english;
    string french;

    word(string &EN, string &FR) {
        english = EN;
        french = FR;
    }
};

vector<word> dictionary;

int parentsockfd;
int bufferSize = 512;

void handleConnection() {
    struct sockaddr_in caddr;
    char incomingBuffer[bufferSize];
    char outgoingBuffer[bufferSize];
    char *body;
    int len;
    bool seqNum = false;
    int wordIndex = -1;

    // receives data from main server
    recvRDT(parentsockfd, caddr, outgoingBuffer, incomingBuffer, bufferSize, len, body);
    if (len < 0) {
        return;
    }
    cout << "Connected to IP: " << inet_ntoa(caddr.sin_addr) << " - Port: " << ntohs(caddr.sin_port) << endl;

    // user request is processed
    for (int i = 0; i < dictionary.size(); i++) {
        if (strncasecmp(body, dictionary[i].english.c_str(), len - 7) == 0) {
            wordIndex = i;
            break;
        }
    }
    if (wordIndex != -1) {
        string translation;
        translation.append(body);
        translation.append(" is translated to ");
        translation.append(dictionary[wordIndex].french);
        strcpy(outgoingBuffer, translation.c_str());
    } else {
        strcpy(outgoingBuffer, "Word not in dictionary");
    }

    // results are sent back to main server
    sendRDT(parentsockfd, caddr, seqNum, outgoingBuffer, incomingBuffer, bufferSize, len);
    if (len < 0) {
        return;
    }
}

void exiting() {
    close(parentsockfd);
    cout << "Exiting";
}

int main() {
    // clean up before program exits
    atexit(exiting);

    // dummy data is loaded
    ifstream english("utility/english.txt");
    ifstream french("utility/french.txt");
    string englishWord;
    string frenchWord;
    while (getline(english, englishWord)) {
        getline(french, frenchWord);
        englishWord.erase(englishWord.size() - 1);
        frenchWord.erase(frenchWord.size() - 1);
        dictionary.emplace_back(englishWord, frenchWord);
    }
    english.close();
    french.close();

    struct sockaddr_in saddr;

    // Another socket is created, which can be used to connect users to the server.
    // socket is set to use ipv4 and tcp
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
    saddr.sin_port = htons(translatorPort);

    // binds main socket to all network interfaces and main port
    if ((bind(parentsockfd, (struct sockaddr *) &saddr, sockSize)) < 0) {
        cout << "Error binding: " << errno << endl;
        close(parentsockfd);
        exit(1);
    }
    cout << "Server is ready on port " << translatorPort << endl << endl;


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
