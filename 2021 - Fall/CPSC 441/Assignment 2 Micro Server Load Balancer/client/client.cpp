// CPSC 441 - Assignment 2 - Client
// made by Kaumil Patel 30088096

#include <iostream>
#include <cstring>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[]) {
    int sockSize = sizeof(struct sockaddr_in);
    int bufferSize = 512;

    unsigned short port;
    char address[15];

    // use command line arguments to get the ip address and port of the server
    if (argc == 3) {
        strcpy(address, argv[1]);
        port = stoi(argv[2]);
    } else {
        cout << "[ip address] [port]" << endl;
        exit(1);
    }

    // Creates a socket which is used to connect to the server
    // socket is set to use ipv4 and tcp
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        cout << "Error creating socket: " << errno << endl;
        close(sockfd);
        exit(1);
    }

    // set server address and port
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    saddr.sin_addr.s_addr = inet_addr(address);

    // set 10 sec timeout
    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char *) &tv, sizeof tv);

    // connect to server using socket sockfd
    if (connect(sockfd, (struct sockaddr *) &saddr, sockSize) == -1) {
        cout << "Cannot connect: " << errno << endl;
        close(sockfd);
        exit(1);
    }

    char buffer[bufferSize];
    int len;
    char menuOption;
    char votingOption;

    // enter main loop
    // asks user for input to navigate menu
    // send and receives data from server
    while (true) {
        cout << "Menu:\r\n"
                "1 - Translator Service\r\n"
                "2 - Currency Converter Service\r\n"
                "3 - Voting Service\r\n"
                "4 - Quit\r\n"
                "Select option: ";
        cin.getline(buffer, bufferSize);
        menuOption = buffer[0];

        if (menuOption < '1' || menuOption > '4') {
            cout << "Error invalid input" << endl;
            continue;
        } else if (menuOption == '4') {
            cout << "Connection Closed" << endl;
            if (send(sockfd, &menuOption, 1, 0) < 0) {
                close(sockfd);
                exit(1);
            }
            close(sockfd);
            exit(0);
        } else {
            if (menuOption == '3') {
                cout << "Voting Menu:\r\n"
                        "1 - get list\r\n"
                        "2 - vote\r\n"
                        "3 - results\r\n";
                if (menuOption < '1' || menuOption > '3') {
                    cout << "Error invalid input" << endl;
                    continue;
                }
                cin.getline(&buffer[1], bufferSize - 1);
                votingOption = buffer[1];
                cout << "Sent: " << buffer << endl;
                if (send(sockfd, buffer, 2, 0) < 0) {
                    cout << "Error: Connection Closed" << endl;
                    close(sockfd);
                    exit(1);
                }
            } else {
                if (send(sockfd, &menuOption, 1, 0) < 0) {
                    cout << "Error: Connection Closed" << endl;
                    close(sockfd);
                    exit(1);
                }
            }

            if (menuOption != '3' || votingOption != '1') {
                if ((len = recv(sockfd, buffer, bufferSize, 0)) < 0) {
                    cout << "Error: Connection Closed" << endl;
                    close(sockfd);
                    exit(1);
                }
            }

            if (menuOption == '3') {
                if (votingOption < '1' || votingOption > '3') {
                    cout << "Error invalid input" << endl;
                    continue;
                } else if (votingOption == '1') {
                    strcpy(buffer, "getList");
                } else {
                    buffer[len] = '\0';
                    cout << "Key: " << buffer << endl;
                    int key = buffer[0] - '0';
                    int id;
                    int option;

                    cout << "enter id" << endl;
                    cin.clear();
                    cin.getline(buffer, bufferSize);
                    cout << buffer << endl;
                    try {
                        id = stoi(buffer);
                        id *= key;
                    } catch (...) {
                        cout << "Enter an integer, try again: ";
                    }

                    if (votingOption == '2') {
                        cout << "enter option" << endl;
                        cin.getline(buffer, bufferSize);
                        try {
                            option = stoi(buffer);
                            option *= key;
                        } catch (...) {
                            cout << "Enter an integer, try again: ";
                        }
                        sprintf(buffer, "vote %d %d", id, option);
                    } else {
                        sprintf(buffer, "getVote %d", id);
                    }
                    cout << option << id << endl;
                    cout << buffer << endl;
                }
            } else {
                buffer[len] = '\0';
                cout << buffer << endl;
                cin.getline(buffer, bufferSize);
            }
            if (menuOption != '3' || votingOption != '1') {
                cout << "Sent: " << buffer << endl;
                if (send(sockfd, buffer, strlen(buffer), 0) < 0) {
                    cout << "Error: Connection Closed" << endl;
                    close(sockfd);
                    exit(1);
                }
            }
            if ((len = recv(sockfd, buffer, bufferSize, 0)) < 0) {
                cout << "Error: Connection Closed" << endl;
                close(sockfd);
                exit(1);
            }
            buffer[len] = '\0';
            cout << buffer << endl;
        }
    }
}
