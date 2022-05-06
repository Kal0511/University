#include <iostream>
#include <cstdlib>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <cctype>
#include <unistd.h>
#include <netdb.h>
#include <vector>
#include "thread"

using namespace std;

bool checkInFilter(char *start, int len);

bool filterURL(string url);

void sendMessage(const int &sockfd, string message);

string receiveMessage(const int &sockfd);

vector<string> filterList;

bool URL_Filter = false;

bool BodyFilter = false;

bool menuStatus = true;

void displayMenu() {
    cout << "Proxy Menu:" << endl << endl;
    cout << "1 - Toggle URL Filter" << endl;
    cout << "2 - Toggle Body Filter" << endl;
    cout << "3 - Filter List & Status" << endl;
    cout << "4 - Add Keyword To Filter" << endl;
    cout << "5 - Remove Keyword From Filter" << endl;
    cout << "6 - Clear Filter" << endl;
    cout << "7 - Quit" << endl << endl;
}

void displayFilter() {
    cout << "Keywords in Filter:" << endl << endl;
    for (int i = 0; i < filterList.size(); i++) {
        cout << i + 1 << " : " << filterList[i] << endl;
    }
    cout << endl;
}

void addKeyword() {
    cout << "Input Keyword to Add: ";
    string keyword;
    cin >> keyword;
    filterList.push_back(keyword);
}

void removeKeyword() {
    cout << "Input Keyword to Add: ";
    int index;
    cin >> index;
    index--;
    if (index < 0 || index >= filterList.size()) {
        cout << "Error: Index Does Not Exists. Please Try Again." << endl;
    }
    filterList.erase(filterList.begin() + index);
}

void selectMenuOption() {
    cout << "Enter Option Number (For Help Input - 8): ";
    int option;
    cin >> option;
    cout << option << endl << endl;
    switch (option) {
        case 1:
            URL_Filter = !URL_Filter;
            if (URL_Filter) {
                cout << "URL Filter Activated" << endl << endl;
            } else {
                cout << "URL Filter Deactivated" << endl << endl;
            }
            break;
        case 2:
            BodyFilter = !BodyFilter;
            if (BodyFilter) {
                cout << "Body Filter Activated" << endl << endl;
            } else {
                cout << "Body Filter Deactivated" << endl << endl;
            }
            break;
        case 3:
            displayFilter();
            break;
        case 4:
            addKeyword();
            break;
        case 5:
            removeKeyword();
            break;
        case 6:
            filterList.clear();
            break;
        case 7:
            exit(0);
        case 8:
            displayMenu();
            break;
        case 0:
            cout << "You Input is Invalid Please Select One of the Following Options:" << endl << endl;
            displayMenu();
            break;
        default:
            cout << "You Input is Invalid Please Select One of the Following Options:" << endl << endl;
            displayMenu();
    }
}

void backgroundMenu() {
    displayMenu();
    while (menuStatus) {
        selectMenuOption();
    }
    cout << "Menu closed" << endl;
}

void HTTP_RequestParser(const string &request, string &method) {

}

int main() {
    // cout << sizeof( long int) << endl;
    // filterList.push_back("floppy");
    char buffer[1024];
    struct sockaddr_in saddr, caddr;
    int sockSize = sizeof(struct sockaddr_in);
    int parentsockfd, childsockfd;
    unsigned short port = 12345;
    if ((parentsockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "Error creating socket: " << errno << endl;
        exit(1);
    }
    memset(&saddr, 0, sockSize);
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(port);
    if ((bind(parentsockfd, (struct sockaddr *) &saddr, sockSize)) < 0) {
        cout << "Error binding: " << errno << endl;
        exit(1);
    }
    if (listen(parentsockfd, 4) < 0) {
        cout << "Error listening: " << errno << endl;
        exit(1);
    }
    cout << "Server is listening on port " << port << endl;

//    int pid = fork();
//    if (pid < 0) {
//        cout << "Fork Error" << endl;
//    } else if (pid > 0) {
//        // Parent process
//        displayMenu();
//        // cin.clear();
//        while (true) {
//            selectMenuOption();
//        }
//    }
//    // Child Process

     std::thread menu(backgroundMenu);
    // menu.detach();
    cout << "After thread start" << endl;

    while (true) {
        if ((childsockfd = accept(parentsockfd, (struct sockaddr *) &caddr, (socklen_t *) &sockSize)) < 0) {
            cout << "Error accepting: " << errno << endl;
            cout << "IP: " << inet_ntoa(caddr.sin_addr) << " - Port: " << ntohs(caddr.sin_port) << endl;
            exit(1);
        }
        cout << "Before fork" << endl;
        int pid = fork();
        cout << "After fork" << endl;

        if (pid < 0) {
            cout << "Fork Error" << endl;
        } else if (pid == 0) {
            menuStatus = false;
            // child process
            close(parentsockfd);
            char incomingMessage[4096];
            char outgoingMessage[4096];
            int incomingMessageLength = recv(childsockfd, incomingMessage, sizeof(incomingMessage), 0);
            buffer[incomingMessageLength] = '\0';

            char method[16];
            char address[128];
            char path[128];
            unsigned short defaultPort = 80;
            int i = 0;
            while (incomingMessage[i] != ' ') {
                method[i] = incomingMessage[i];
                i++;
            }
            i++;
            if (strcmp(method, "CONNECT") == 0) {
                //char badRequestResponse[] = "HTTP/1.1 400 Bad Request\n\n";

                string connectResponse = "HTTP/1.1 200 Connection Established\r\nConnection: close\r\n\r\n";
                // cout << strlen(connectResponse) << endl;
                sendMessage(childsockfd, connectResponse);

//                close(childsockfd);
//                exit(0);
                int offset = i;
                while (incomingMessage[i] != ' ') {
                    if (incomingMessage[i] == ':') {
                        i++;
                        defaultPort = 0;
                        while (incomingMessage[i] != ' ') {
                            defaultPort *= 10;
                            defaultPort += incomingMessage[i] - '0';
                            i++;
                        }
                        break;
                    }
                    address[i - offset] = incomingMessage[i];
                    i++;
                }
            } else if (strcmp(method, "GET") == 0) {
                // cout << &incomingMessage[i] << endl;
                if (strncmp(&incomingMessage[i], "http://", 7) == 0) {
                    i += 7;
                }
                int offset = i;
                while (incomingMessage[i] != ' ' && incomingMessage[i] != '/') {
                    address[i - offset] = incomingMessage[i];
                    path[i - offset] = incomingMessage[i];
                    i++;
                }
                i--;
                while (incomingMessage[i] != ' ') {
                    path[i - offset] = incomingMessage[i];
                    i++;
                }
                // cout << path << endl;
            }
            if (filterURL(string(path))) {
                cout << path << endl;
            }
            struct addrinfo hints, *result;
            memset(&hints, 0, sizeof(hints));
            hints.ai_family = AF_INET;
            hints.ai_socktype = SOCK_STREAM;
            hints.ai_protocol = IPPROTO_TCP;
            int errorCode = getaddrinfo(address, NULL, &hints, &result);
            if (errorCode != 0) {
                cout << "address info error" << endl;
                exit(0);
            }
//            struct sockaddr_in saddr;
//            char buffer[128];
//            const char *address = "136.159.5.25";
//            saddr.sin_family = result->ai_family;
//            saddr.sin_port = htons(port);
//            saddr.sin_addr.s_addr = inet_addr(result->ai_addr->sa_data);
// cout << result->ai_family << result->ai_socktype << result->ai_protocol << endl;
            struct sockaddr_in *addr4 = (struct sockaddr_in *) result->ai_addr;
//            inet_ntop(AF_INET, &addr4->sin_addr, buffer, sizeof(buffer));
            addr4->sin_port = htons(defaultPort);
//            cout << result->ai_addr << endl;
            int sockfd;
            if ((sockfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol)) == -1) {
                //if ((sockfd = socket(AF_INET, SOCK_STREAM,0)) == -1) {
                std::cout << "Error creating socket" << std::endl;
                std::cout << errno << std::endl;
                exit(1);
            }
            int connfd;
            if ((connfd = connect(sockfd, (struct sockaddr *) addr4, sockSize)) == -1) {
                std::cout << "Cannot connect" << std::endl;
                std::cout << errno << std::endl;
                exit(1);
            }
            // int len = recv(sockfd, message, sizeof(message), 0);
            // cout << message << endl;
            // cout << buffer << endl;
            // cout << incomingMessage << endl;
            // cout << address << endl;
            if (strcmp(method, "CONNECT") == 0) {
                while (true) {
                    string client = receiveMessage(childsockfd);
                    sendMessage(sockfd, client);
                    string server = receiveMessage(sockfd);
                    sendMessage(childsockfd, server);
                }
            } else if (strcmp(method, "GET") == 0) {
                send(sockfd, incomingMessage, incomingMessageLength, 0);
                //while (true) {
                //int len = recv(sockfd, incomingMessage, sizeof(incomingMessage), 0);
                string message = receiveMessage(sockfd);
                //cout << message.c_str() << endl;
                // cout << "start: " << incomingMessage << " :end" << endl;
//                        if (checkInFilter(&message[0], message.length()) &&
//                            strcmp(path, "pages.cpsc.ucalgary.ca/~carey/CPSC441/ass1/error.html") != 0) {
//                            cout << true << endl;
//                            char error[] = "HTTP/1.1 301 Moved Permanently\n"
//                                           "Location: http://pages.cpsc.ucalgary.ca/~carey/CPSC441/ass1/error.html\n";
//                            send(childsockfd, error, strlen(error), 0);
//                            close(sockfd);
//                            close(childsockfd);
//                            exit(0);
                //                       }
//                        if (len == 0) {
//                            break;
//                        }
                //send(childsockfd, incomingMessage, len, 0);
                //send(childsockfd,&message[0], message.length(),0);

                sendMessage(childsockfd, message);
                //}
                // int incomingMessageLength = recv(childsockfd, incomingMessage, sizeof(incomingMessage), 0);
                // cout << incomingMessage << endl;

            }

//            send(sockfd, message, strlen(message), 0);
//            int len = recv(sockfd, message, sizeof(message), 0);
//            message[len] = '\0';
//            // cout << len << " : " << strlen(message) << endl;
//            // cout << "Start:" << message << ":End" << endl;
//            // cout << buffer << endl;
            close(sockfd);
//
//            send(childsockfd, message, strlen(message), 0);

            close(childsockfd);
            exit(0);
        } else {
// parent process
            close(childsockfd);
        }
    }
    return 0;
}


bool checkInFilter(char *start, int len) {
    bool inFilter = false;
    for (int n = 0; n < len; n++) {
        while (!isalpha(start[n])) {
            n++;
        }
        for (int i = 0; i < filterList.size(); i++) {
            if (strncasecmp(&start[n], &filterList[i][0], strlen(&filterList[i][0])) == 0) {
                inFilter = true;
                break;
            }
        }
        if (inFilter) {
            break;
        }
        while (isalpha(start[n])) {
            n++;
        }
    }
    return inFilter;
}

string receiveMessage(const int &sockfd) {
    char incomingMessageBuffer[1024];
    string incomingMessage = "";
    int len;
    while (true) {
        if ((len = recv(sockfd, incomingMessageBuffer, sizeof(incomingMessageBuffer), 0)) == -1) {
            close(sockfd);
            exit(-1);
        } else if (len == 0) {
            return incomingMessage;
        } else {
            incomingMessage.append(incomingMessageBuffer, len);
        }
    }
}

void sendMessage(const int &sockfd, string message) {
    int len = 0;
    while (len < message.length()) {
        if ((len += send(sockfd, message.c_str(), message.length() - len, 0)) == -1) {
            close(sockfd);
            exit(-1);
        }
    }
}

bool filterURL(string url) {
    bool inFilter = false;
    for (int n = 0; n < url.length(); n++) {
        while (!isalpha(url[n])) {
            n++;
        }
        for (int i = 0; i < filterList.size(); i++) {
            if (strncasecmp(&url[n], &filterList[i][0], strlen(&filterList[i][0])) == 0) {
                inFilter = true;
                break;
            }
        }
        if (inFilter) {
            break;
        }
    }
    return inFilter;
}


