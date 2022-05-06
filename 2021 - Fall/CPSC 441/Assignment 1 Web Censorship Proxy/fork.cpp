#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <vector>
#include <thread>
#include <algorithm>

// DEBUG 1
// #define bufferSize 4096

using namespace std;

vector<string> filterList;

bool URL_Filter = false;
bool BodyFilter = false;
bool menuFlag = true;
bool exitFlag = true;

int parentsockfd;

int sockSize = sizeof(struct sockaddr_in);

bool checkInFilter(const string &message);

bool filterURL(const string &url);

void sendMessage(const int &sockfd, const string &message);

string receiveMessage(const int &sockfd);

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
    if (URL_Filter) {
        cout << "URL Filter Active" << endl << endl;
    } else {
        cout << "URL Filter Inactive" << endl << endl;
    }
    if (BodyFilter) {
        cout << "Body Filter Active" << endl << endl;
    } else {
        cout << "Body Filter Inactive" << endl << endl;
    }
    if (filterList.size() > 0) {
        cout << filterList.size() << " Keywords in Filter:" << endl << endl;
        for (int i = 0; i < filterList.size(); i++) {
            cout << i + 1 << " : " << filterList[i] << endl;
        }
        cout << endl;
    } else {
        cout << "Filter is Empty" << endl << endl;
    }
}

void addKeyword() {
    cout << "Input Keyword to Add: ";
    string keyword;
    cin >> keyword;
    cout << endl;
    filterList.push_back(keyword);
}

void removeKeyword() {
    cout << "Input Keyword to Add: ";
    int index;
    cin >> index;
    cout << endl;
    index--;
    if (index < 0 || index >= filterList.size()) {
        cout << "Error: Index Does Not Exists. Please Try Again." << endl;
    }
    filterList.erase(filterList.begin() + index);
}

void selectMenuOption() {
    cout << "Enter Option Number (For Help Input - 8): ";
    char option;
    cin >> option;
    cout << endl;
    switch (option) {
        case '1':
            URL_Filter = !URL_Filter;
            if (URL_Filter) {
                cout << "URL Filter Activated" << endl << endl;
            } else {
                cout << "URL Filter Deactivated" << endl << endl;
            }
            break;
        case '2':
            BodyFilter = !BodyFilter;
            if (BodyFilter) {
                cout << "Body Filter Activated" << endl << endl;
            } else {
                cout << "Body Filter Deactivated" << endl << endl;
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
            cout << "You Input is Invalid. Please Select One of the Following Options:" << endl << endl;
            displayMenu();
    }
}

void backgroundMenu() {
    displayMenu();
    while (menuFlag) {
        selectMenuOption();
    }
    cout << "Menu Closed" << endl;
}

void HTTP_RequestParser(const string &request, string &method, string &path, string &version, string &host) {
    int i = 0;
    int start = 0;
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
        if ((len = recv(sender, buffer, sizeof(buffer), 0)) == -1) {
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


void connection(const int &childsockfd) {
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

    struct addrinfo hints, *result;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    if (getaddrinfo(host.c_str(), "http", &hints, &result) != 0) {
        cout << "address info error" << endl;
    }

    struct sockaddr_in *addr4 = (struct sockaddr_in *) result->ai_addr;
    // addr4->sin_port = htons(defaultPort);

    int sockfd;
    if ((sockfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol)) == -1) {
        std::cout << "Error creating socket" << std::endl;
        std::cout << errno << std::endl;
        return;
    }

    int connfd;
    if ((connfd = connect(sockfd, (struct sockaddr *) addr4, sockSize)) == -1) {
        std::cout << "Cannot connect" << std::endl;
        std::cout << errno << std::endl;
        return;
    }

    sendMessage(sockfd, incomingMessage);

    if (BodyFilter) {
        string message = receiveMessage(sockfd);
        if (checkInFilter(message) &&
            strcmp(path.c_str(), "http://pages.cpsc.ucalgary.ca/~carey/CPSC441/ass1/error.html") != 0) {
            string error = "HTTP/1.0 301 Moved Permanently\r\nLocation: http://pages.cpsc.ucalgary.ca/~carey/CPSC441/ass1/error.html\r\n\r\n";
            sendMessage(childsockfd, error);
        } else {
            sendMessage(childsockfd, message);
        }
    } else {
        oneWayTunnel(sockfd, childsockfd);
    }
    close(sockfd);
    close(childsockfd);
}
int main() {
    struct sockaddr_in saddr, caddr;
    int sockSize = sizeof(struct sockaddr_in);
    int configsockfd, childsockfd;
    unsigned short port = 12345;
    unsigned short configPort = 12346;

    if ((configsockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "Error creating socket: " << errno << endl;
        exit(1);
    }
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
    if ((childsockfd = accept(parentsockfd, (struct sockaddr *) &caddr, (socklen_t *) &sockSize)) < 0) {
        cout << "Error accepting: " << errno << endl;
        cout << "IP: " << inet_ntoa(caddr.sin_addr) << " - Port: " << ntohs(caddr.sin_port) << endl;
        exit(1);
    }

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
    if (listen(parentsockfd, 4) < 0) {
        cout << "Error listening: " << errno << endl;
        exit(1);
    }
    cout << "Server is listening on port " << port << endl;

    std::thread menu(backgroundMenu);

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
        if ((childsockfd = accept(parentsockfd, (struct sockaddr *) &caddr, (socklen_t *) &sockSize)) < 0) {
            if(errno == 22){
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


