#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <vector>
#include <thread>
#include <algorithm>

// DEBUG 1
// #define bufferSize 4096

// telnet pages.cpsc.ucalgary.ca 80
// telnet 192.168.0.50 12345
// GET http://pages.cpsc.ucalgary.ca/~carey/CPSC441/assignment1.html HTTP/1.0

using namespace std;

vector<string> filterList;

bool URL_Filter = false;
bool BodyFilter = false;
bool menuFlag = true;
bool exitFlag = true;

char buffer[128];

int parentsockfd, configsockfd, thatsock;

int sockSize = sizeof(struct sockaddr_in);

string receiveMessageNonBlocking(int &sockfd);

bool checkInFilter(const string &message);

bool filterURL(const string &url);

void sendMessage(const int &sockfd, const string &message);

string receiveMessage(const int &sockfd);

void displayMenu() {
    string menu = "Proxy Menu:\r\n"
                  "1 - Toggle URL Filter\r\n"
                  "2 - Toggle Body Filter\r\n"
                  "3 - Filter List & Status\r\n"
                  "4 - Add Keyword To Filter\r\n"
                  "5 - Remove Keyword From Filter\r\n"
                  "6 - Clear Filter\r\n"
                  "7 - Quit\r\n\n";
    sendMessage(thatsock, menu);
}

void displayFilter() {
    if (URL_Filter) {
        sendMessage(thatsock, "URL Filter Active\r\n\r\n");
        // cout << "URL Filter Active" << endl << endl;
    } else {
        sendMessage(thatsock, "URL Filter Inactive\r\n\r\n");
        // cout << "URL Filter Inactive" << endl << endl;
    }
    if (BodyFilter) {
        sendMessage(thatsock, "Body Filter Active\r\n\r\n");
        //cout << "Body Filter Active" << endl << endl;
    } else {
        sendMessage(thatsock, "Body Filter Inactive\r\n\r\n");
        //cout << "Body Filter Inactive" << endl << endl;
    }
    if (filterList.size() > 0) {
        // cout << filterList.size() << " Keywords in Filter:" << endl << endl;
        sprintf(buffer, "%d Keywords in Filter:\r\n", filterList.size());
        sendMessage(thatsock, buffer);
        for (int i = 0; i < filterList.size(); i++) {
            sprintf(buffer, "%d : %s", i + 1, filterList[i].c_str());
            sendMessage(thatsock, buffer);
            //cout << i + 1 << " : " << filterList[i] << endl;
        }
        sendMessage(thatsock, "\r\n");
        // cout << endl;
    } else {
        sendMessage(thatsock, "Filter is Empty\r\n\r\n");
        // cout << "Filter is Empty" << endl << endl;
    }
}

void addKeyword() {
    sendMessage(thatsock, "Input Keyword to Add: ");
    //cout << "Input Keyword to Add: ";
    int len = recv(thatsock, buffer, sizeof(buffer), 0);
    buffer[len + 1] = '\0';
    sendMessage(thatsock, "\r\n\r\n");
    string keyword(buffer);
    // cin >> keyword;
    cout << endl;
    filterList.push_back(keyword);
}

void removeKeyword() {
    sendMessage(thatsock, "Input Index of Keyword to Remove: ");
    //cout << "Input Index of Keyword to Remove: ";
    int len = recv(thatsock, buffer, sizeof(buffer), 0);
    buffer[len + 1] = '\0';
    sendMessage(thatsock, "\r\n\r\n");
    int index = buffer[0] - '0';
    // cin >> index;
    cout << endl;
    index--;
    if (index < 0 || index >= filterList.size()) {
        sendMessage(thatsock, "Error: Index Does Not Exists. Please Try Again.\r\n\r\n");
        //cout << "Error: Index Does Not Exists. Please Try Again." << endl;
        return;
    }
    filterList.erase(filterList.begin() + index);
}

void selectMenuOption() {
    // cout << "Enter Option Number (For Help Input - 8): ";
    sendMessage(thatsock, "Enter Option Number (For Help Input - 8): ");
    // cout << receiveMessage(configsockfd) << endl;
    int len = recv(thatsock, buffer, sizeof(buffer), 0);
    buffer[len+1] = '\0';
    sendMessage(thatsock, "\r\n\r\n");
    cout << buffer << endl;
    // char option = receiveMessageNonBlocking(configsockfd)[0];
    char option = buffer[0];
    // cin >> option;
    cout << endl;
    switch (option) {
        case '1':
            URL_Filter = !URL_Filter;
            if (URL_Filter) {
                sendMessage(thatsock, "URL Filter Activated\r\n\r\n");
                //cout << "URL Filter Activated" << endl << endl;
            } else {
                sendMessage(thatsock, "URL Filter Deactivated\r\n\r\n");
                //cout << "URL Filter Deactivated" << endl << endl;
            }
            break;
        case '2':
            BodyFilter = !BodyFilter;
            if (BodyFilter) {
                sendMessage(thatsock, "Body Filter Activated\r\n\r\n");
                // cout << "Body Filter Activated" << endl << endl;
            } else {
                sendMessage(thatsock, "Body Filter Deactivated\r\n\r\n");
                // cout << "Body Filter Deactivated" << endl << endl;
            }
            break;
        case '3':
            displayFilter();
            break;
        case '4':
            addKeyword();
            break;
        case '5':
            removeKeyword();
            break;
        case '6':
            filterList.clear();
            break;
        case '7':
            exitFlag = false;
            menuFlag = false;
            // shutdown(parentsockfd, SHUT_RDWR);
            close(parentsockfd);
            break;
        case '8':
            displayMenu();
            break;
        default:
            sendMessage(thatsock, "You Input is Invalid. Please Select One of the Following Options:\r\n");
            displayMenu();
    }
}


void backgroundMenu() {
    displayMenu();
    recv(thatsock, buffer, sizeof(buffer), 0);
    memset(buffer, '\0', sizeof(buffer));
    while (menuFlag) {
        selectMenuOption();
    }
    cout << "Menu Closed" << endl;
}

void HTTP_RequestParser(const string &request, string &method, string &path, string &version, string &host) {
    int i = 0;
    while (!isalpha(request[i])) {
        i++;
    }
    int start = i;
    while (request[i] != ' ') {
        i++;
    }
    method = request.substr(start, i - start);
    i++;
    start = i;
    while (request[i] != ' ') {
        i++;
    }
    path = request.substr(start, i - start);
    i++;
    start = i;
    while (request[i] != '\r') {
        i++;
    }
    version = request.substr(start, i - start);
    start = request.find("Host");
    if (start != std::string::npos) {
        start += 6;
        i = start;
        while (request[i] != '\r') {
            i++;
        }
        host = request.substr(start, i - start);
    } else {
        i = 0;
        if (strncasecmp(&path[0], "http://", 7) == 0) {
            i += 7;
        }
        start = i;
        while (path[i] != '/' && path[i] != ' ') {
            i++;
        }
        host = path.substr(start, i - start);
    }
}

void HTTP_ResponseParser(const string &response, string &contentType) {

}

void oneWayTunnel(const int &sender, const int &receiver) {
    char buffer[4096];
    int len;
    while (true) {
        len = recv(sender, buffer, sizeof(buffer), 0);
        buffer[len + 1] = '\0';
        if (len == -1) {
            cout << "Error Receiving Data" << endl;
            close(sender);
            close(receiver);
            return;
        } else if (len == 0) {
            return;
        }
        if (send(receiver, buffer, len, 0) == -1) {
            cout << "Error Sending Data" << endl;
            close(sender);
            close(receiver);
            return;
        }
    }
}


void connection(const int childsockfd) {
    string incomingMessage = receiveMessage(childsockfd);
    // incomingMessage = "GET http://pages.cpsc.ucalgary.ca/~carey/CPSC441/ass1/test1.html HTTP/1.0\r\n\r\n";

    string method;
    string path;
    string version;
    string host;
    HTTP_RequestParser(incomingMessage, method, path, version, host);

    if (filterURL(path) && URL_Filter) {
        string error = "HTTP/1.1 301 Moved Permanently\r\nLocation: http://pages.cpsc.ucalgary.ca/~carey/CPSC441/ass1/error.html\r\n\r\n";
        sendMessage(childsockfd, error);
        close(childsockfd);
        return;
    }
    cout << host.c_str() << endl;
    struct addrinfo hints, *result;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    if (getaddrinfo(host.c_str(), "http", &hints, &result) != 0) {
        cout << "address info error" << endl;
        close(childsockfd);
        return;
    }

    struct sockaddr_in *addr4 = (struct sockaddr_in *) result->ai_addr;
    // addr4->sin_port = htons(defaultPort);

    int sockfd;
    if ((sockfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol)) == -1) {
        std::cout << "Error creating socket" << std::endl;
        std::cout << errno << std::endl;
        close(childsockfd);
        return;
    }

    int connfd;
    if ((connfd = connect(sockfd, (struct sockaddr *) addr4, sockSize)) == -1) {
        std::cout << "Cannot connect" << std::endl;
        std::cout << errno << std::endl;
        close(childsockfd);
        close(sockfd);
        return;
    }

    sendMessage(sockfd, incomingMessage);

//    if (BodyFilter) {
        string message = receiveMessage(sockfd);
        close(sockfd);
        if (checkInFilter(message) &&
            strcmp(path.c_str(), "http://pages.cpsc.ucalgary.ca/~carey/CPSC441/ass1/error.html") != 0) {
            string error = "HTTP/1.0 301 Moved Permanently\r\nLocation: http://pages.cpsc.ucalgary.ca/~carey/CPSC441/ass1/error.html\r\n\r\n";
            sendMessage(childsockfd, error);
        } else {
            sendMessage(childsockfd, message);
        }
//    } else {
//        oneWayTunnel(sockfd, childsockfd);
//    }
    close(childsockfd);
}

int main() {
    struct sockaddr_in saddr, caddr;
    int sockSize = sizeof(struct sockaddr_in);
    // int childsockfd;
    unsigned short port = 12345;
    unsigned short configPort = 12346;

    if ((configsockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "Error creating socket: " << errno << endl;
        exit(1);
    }

    struct timeval timeout;
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
//    if (setsockopt(configsockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout) < 0) {
//        cout << "setsockopt failed\n";
//    }

    memset(&saddr, 0, sockSize);
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(configPort);
    if ((bind(configsockfd, (struct sockaddr *) &saddr, sockSize)) < 0) {
        cout << "Error binding: " << errno << endl;
        exit(1);
    }
    if (listen(configsockfd, 1) < 0) {
        cout << "Error listening: " << errno << endl;
        exit(1);
    }

    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(configsockfd, &rfds);
    int recVal = select(configsockfd + 1, &rfds, NULL, NULL, &timeout);
    if (recVal < 0) {
        cout << "Error accepting: " << errno << endl;
    } else if (recVal == 0) {

        cout << "timeout\n";
    } else if (recVal > 0) {
        if ((thatsock = accept(configsockfd, (struct sockaddr *) &caddr, (socklen_t *) &sockSize)) < 0) {
            cout << "Error accepting: " << errno << endl;
            cout << "IP: " << inet_ntoa(caddr.sin_addr) << " - Port: " << ntohs(caddr.sin_port) << endl;
            exit(1);
        }
        sendMessage(thatsock, "Connected\r\n\r\n");
        std::thread menu(backgroundMenu);
        menu.detach();
    }


//    int flags = fcntl(configsockfd,F_GETFL,0);
//    fcntl(configsockfd, F_SETFL, flags | O_NONBLOCK);


    if ((parentsockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "Error creating socket: " << errno << endl;
        exit(1);
    }



//	struct timeval timeout;
//    timeout.tv_sec = 5;
//    timeout.tv_usec = 0;
//	if (setsockopt (parentsockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout) < 0){
//        cout << "setsockopt failed\n";
//	}

    memset(&saddr, 0, sockSize);
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(port);
    if ((bind(parentsockfd, (struct sockaddr *) &saddr, sockSize)) < 0) {
        cout << "Error binding: " << errno << endl;
        exit(1);
    }
    if (listen(parentsockfd, 16) < 0) {
        cout << "Error listening: " << errno << endl;
        exit(1);
    }
    cout << "Server is listening on port " << port << endl;


    while (exitFlag) {
//		fd_set set;
//		FD_ZERO(&set);
//		FD_SET(parentsockfd, &set);
//		int status = select(parentsockfd + 1, &set, NULL, NULL, &timeout);
//		if (status == -1) {
//			cout << "Error durnig select" << endl;
//			exit(1);
//		}else if (status == 0){
//			cout << "timeout occurred (20 second) \n";
//			continue;
//		}
        int childsockfd;
        if ((childsockfd = accept(parentsockfd, (struct sockaddr *) &caddr, (socklen_t *) &sockSize)) < 0) {
            if (errno == 22) {
                cout << "Stopped accepting new connections" << endl;
                exit(0);
            }
            cout << "Error accepting: " << errno << endl;
            cout << "IP: " << inet_ntoa(caddr.sin_addr) << " - Port: " << ntohs(caddr.sin_port) << endl;
            exit(1);
        }

        std::thread request(connection, childsockfd);
        request.detach();
    }
    close(parentsockfd);
    return 0;
}


bool checkInFilter(const string &message) {
    if (filterList.size() == 0) {
        return false;
    }
    for (int n = 0; n < message.length(); n++) {
        while (!isalpha(message[n])) {
            n++;
        }
        for (int i = 0; i < filterList.size(); i++) {
            if (strncasecmp(&message[n], &filterList[i][0], filterList[i].length()) == 0) {
                return true;
            }
        }
        while (isalpha(message[n])) {
            n++;
        }
    }
    return false;
}

string receiveMessageNonBlocking(int &sockfd) {
    char incomingMessageBuffer[4096];
    string incomingMessage = "";
    int len;
    while (true) {
        if ((len = recv(sockfd, incomingMessageBuffer, sizeof(incomingMessageBuffer), 0)) <= 0) {
            return incomingMessage;
        } else {
            incomingMessage.append(incomingMessageBuffer, len);
        }
    }
}

string receiveMessage(const int &sockfd) {
    char incomingMessageBuffer[4096];
    string incomingMessage = "";
    int len;
    while (true) {
        if ((len = recv(sockfd, incomingMessageBuffer, sizeof(incomingMessageBuffer), 0)) == -1) {
            cout << "Error Receiving Data" << endl;
            close(sockfd);
            return incomingMessage;
        } else if (len == 0) {
            return incomingMessage;
        } else {
            incomingMessage.append(incomingMessageBuffer, len);
            int index = incomingMessage.find("\r\n\r\n");
            if (index != std::string::npos && (index + 4) == incomingMessage.length()) {
                return incomingMessage;
            }
        }
    }
}

void sendMessage(const int &sockfd, const string &message) {
//    int len = 0;
//    while (len < message.length()) {
//        if ((len += send(sockfd, &message[len], message.length() - len, 0)) == -1) {
//            close(sockfd);
//            cout << "Error Sending Data" << endl;
//            return;
//        }
//    }
    if (send(sockfd, message.c_str(), message.length(), 0) == -1) {
        close(sockfd);
        cout << "Error Sending Data" << endl;
        return;
    }
}

bool filterURL(const string &url) {
    if (filterList.size() == 0) {
        return false;
    }
    for (int n = 0; n < url.length(); n++) {
        while (!isalpha(url[n])) {
            n++;
        }
        for (int i = 0; i < filterList.size(); i++) {
            if (strncasecmp(&url[n], filterList[i].c_str(), filterList[i].length()) == 0) {
                return true;
            }
        }
    }
    return false;
}


