// CPSC 441 - Assignment 2 - Voting Service
// made by Kaumil Patel 30088096

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <vector>
#include <thread>
#include <algorithm>
#include <ctime>

#include "../utility/rdt.h"

// Voting Service
#define votingPort 12348

using namespace std;

struct person {
    int ID;
    string name;
    int option;

    person(int ID, string name) {
        this->ID = ID;
        this->name = std::move(name);
        this->option = -1;
    }
};

int numofCandiates = 4;

vector<person> list;
vector<int> results(numofCandiates);
int parentsockfd;
int bufferSize = 512;

time_t votingTime;
time_t resultTime;

void handleConnection() {
    struct sockaddr_in caddr;
    bool seqNum = false;
    time_t rawtime;

    char incomingBuffer[bufferSize];
    char outgoingBuffer[bufferSize];
    char *body;
    int len;

    // receives data from main server
    recvRDT(parentsockfd, caddr, outgoingBuffer, incomingBuffer, bufferSize, len, body);
    if (len < 0) {
        cout << "timeout" << endl;
        return;
    }
    int key = ntohs(caddr.sin_port) % 7 + 2;

    cout << "Connected to IP: " << inet_ntoa(caddr.sin_addr) << " - Port: " << ntohs(caddr.sin_port) << endl;

    // user request is processed
    cout << "Got Body: " << body << endl;
    if (strncasecmp(body, "getList", 7) == 0) {
        string candidates = "Candidates List:\r\n";
        for (int i = 0; i < list.size(); i++) {
            sprintf(outgoingBuffer, "ID: [%d] - Name: [%s]\r\n", list[i].ID, list[i].name.c_str());
            candidates.append(outgoingBuffer);
        }
        strcpy(outgoingBuffer, candidates.c_str());
    } else if (strncasecmp(body, "vote", 4) == 0) {
        cout << time(&rawtime) << " " << votingTime << endl;
        if (time(&rawtime) > votingTime) {
            strcpy(outgoingBuffer, "voting time over");
        } else {

            int ID;
            int option;
            int start = 5;
            int i = start;
            ID = stoi(&body[start]) / key;
            if (isdigit(body[i])) {
                i++;
            }
            i++;
            start = i;
            option = stoi(&body[start]) / key - 1;

            cout << ID << " " << option << endl;
            if (option < 0 || option >= numofCandiates) {
                strcpy(outgoingBuffer, "Invalid vote");
            } else {
                bool exists = false;
                for (i = 0; i < list.size(); i++) {
                    if (ID == list[i].ID) {
                        if (list[i].option == -1) {
                            list[i].option = option;
                            results[option]++;
                        } else {
                            results[list[i].option]--;
                            list[i].option = option;
                            results[option]++;
                        }
                        exists = true;
                        break;
                    }
                }
                if (exists) {
                    strcpy(outgoingBuffer, "vote registered");
                } else {
                    strcpy(outgoingBuffer, "invalid vote");
                }
            }
        }
    } else if (strncasecmp(body, "getVote", 7) == 0) {
        if (time(&rawtime) < resultTime) {
            strcpy(outgoingBuffer, "voting time is not over");
        } else {

            int ID;
            int i = 8;
            ID = stoi(&body[i]) / key;

            cout << ID << endl;

            bool exists = false;
            for (i = 0; i < list.size(); i++) {
                if (ID == list[i].ID) {
                    if (list[i].option != -1) {
                        exists = true;
                    }
                    break;
                }
            }
            if (exists) {
                string resultout = "Results:";
                for (i = 0; i < results.size(); i++) {
                    sprintf(outgoingBuffer, "\r\nCandidate %d got %d votes", i + 1, results[i]);
                    resultout.append(outgoingBuffer);
                }
                strcpy(outgoingBuffer, resultout.c_str());
            } else {
                strcpy(outgoingBuffer, "vote first");
            }

        }
    } else if (strncasecmp(body, "getKey", 6) == 0) {
        outgoingBuffer[0] = key + '0';
        outgoingBuffer[1] = '\0';
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
    exit(0);
}

int main() {
    // clean up before program exits
    atexit(exiting);

    // dummy data for voting
    list.emplace_back(1, "Harry");
    list.emplace_back(2, "Mary");
    list.emplace_back(3, "Jordan");
    list.emplace_back(4, "Sarah");
    list.emplace_back(5, "Don");


    struct tm t;

    // set time limit for voting
    t.tm_year = 2022 - 1900;
    t.tm_mon = 10 - 1;
    t.tm_mday = 22;
    t.tm_hour = 15 - 1;
    t.tm_min = 0;
    t.tm_sec = 0;
    votingTime = mktime(&t);

    // set time for viewing results
    t.tm_year = 2021 - 1900;
    t.tm_mon = 10 - 1;
    t.tm_mday = 22;
    t.tm_hour = 15 - 1;
    t.tm_min = 0;
    t.tm_sec = 0;
    resultTime = mktime(&t);


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
    saddr.sin_port = htons(votingPort);

    // binds main socket to all network interfaces and main port
    if ((bind(parentsockfd, (struct sockaddr *) &saddr, sockSize)) < 0) {
        cout << "Error binding: " << errno << endl;
        close(parentsockfd);
        exit(1);
    }
    cout << "Server is ready on port " << votingPort << endl << endl;

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
            cout << "new connection" << endl;
            thread child(handleConnection);
            child.detach();
        }
    }

}
