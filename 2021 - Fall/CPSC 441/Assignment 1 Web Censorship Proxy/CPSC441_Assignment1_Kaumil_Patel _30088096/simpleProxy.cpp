// Simple Proxy Server
// made by Kaumil Patel 30088096

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

// define the ports the users and admin uses to connect to the server
#define proxyPort 12345
#define configPort 12346

using namespace std;

int sockSize = sizeof(struct sockaddr_in);
vector<string> filterList;
bool URL_Filter = false;
bool BodyFilter = false;
bool menuFlag = true;
char buffer[128];
int parentsockfd, adminsockfd;

// Function takes in a string response and checks if it contains keywords from the filter list. Returns true is contain keyword else false.
bool filterBody(const string &message);
// Function takes in a string path and checks if it contains keywords from the filter list. Returns true is contain keyword else false.
bool filterURL(const string &url);

// Function takes in a socket file descriptor and a string, sends the string to the client the socket is connected to.
void sendMessage(const int &sockfd, const string &message);
// Function takes in a socket file descriptor, and retrieves what the client is sending through the socket. Returns the message acquired as a string.
string receiveMessage(const int &sockfd);

// Function display the config menu to the admin.
void displayMenu();
// Function display the filter status of the url and the body and the keywords in the filter list to the admin.
void displayFilter();
// Function adds keywords to the list sent be the admin
void addKeyword();
// Function removes keywords from the list sent be the admin
void removeKeyword();
// Function asks the admin to select a menu option
void selectMenuOption();
// Function start the menu loop
void startMenu();

// Function takes in a string request which is parsed for information. Retrieves the method, path, version, and host from the request and returns them as an argument of the function.
void HTTP_RequestParser(const string &request, string &method, string &path, string &version, string &host);
// Function takes in a string response which is parsed for information. Retrieves the contentType and contentLength from the response and returns them as an argument of the function.
void HTTP_ResponseParser(const string &response, string &contentType, int &contentLength);

// Function takes in the file descriptor of the client
// Parses the request and filters the urls if url filter is active
// If request contains bad content then client is redirected to error page else
// Connects to the requested server
// Gets the response from the server
// Parses the response and filters the body if body filter is active
// If request contains bad content then client is redirected to error page else
// Sends the response to the client and closes connections
void connection(int childsockfd);

int main() {
    struct sockaddr_in saddr, caddr;
    int configsockfd;

    // 10 second timeout is set for admin to join
    struct timeval timeout;
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;


    cout << "Simple Proxy server started" << endl << endl;
    cout << "Admin has 10 seconds to connect on port " << configPort << endl << endl;
    // Creates a socket which is used to connect admins to the server
    // socket is set to use ipv4 and tcp
    if ((configsockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "Error creating socket: " << errno << endl;
        exit(1);
    }
    memset(&saddr, 0, sockSize);
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(configPort);
    // binds config socket to all network interfaces and config port
    if ((bind(configsockfd, (struct sockaddr *) &saddr, sockSize)) < 0) {
        cout << "Error binding: " << errno << endl;
        exit(1);
    }
    // sets config socket to accept client connection requests
    if (listen(configsockfd, 1) < 0) {
        cout << "Error listening: " << errno << endl;
        exit(1);
    }
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(configsockfd, &rfds);
    // select is used to wait 10 secs for the admin to join the server
    int recVal = select(configsockfd + 1, &rfds, NULL, NULL, &timeout);
    if (recVal < 0) {
        cout << "Error selecting: " << errno << endl;
    } else if (recVal == 0) {
        cout << "Admin did not join" << endl;
        cout << "Config menu did not start" << endl << endl;
        close(configsockfd);
    } else if (recVal > 0) {
        // accepts the admin's connection and creates a new file descriptor for that connection
        if ((adminsockfd = accept(configsockfd, (struct sockaddr *) &caddr, (socklen_t *) &sockSize)) < 0) {
            cout << "Error accepting: " << errno << endl;
            cout << "IP: " << inet_ntoa(caddr.sin_addr) << " - Port: " << ntohs(caddr.sin_port) << endl;
            exit(1);
        }
        close(configsockfd);
        // Since the admin had connected to the server, a menu thread is created with handles the admin changing the server configuration
        sendMessage(adminsockfd, "Connected to Simple Proxy Server\r\n\r\n");
        std::thread menu(startMenu);
        menu.detach();
    }

    // Another socket is created, which can be used to connect users to the server.
    // socket is set to use ipv4 and tcp
    if ((parentsockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "Error creating socket: " << errno << endl;
        exit(1);
    }
    memset(&saddr, 0, sockSize);
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(proxyPort);
    // binds main socket to all network interfaces and main port
    if ((bind(parentsockfd, (struct sockaddr *) &saddr, sockSize)) < 0) {
        cout << "Error binding: " << errno << endl;
        exit(1);
    }
    // sets parent socket to accept client connection requests and had a listening queue of 16
    if (listen(parentsockfd, 16) < 0) {
        cout << "Error listening: " << errno << endl;
        exit(1);
    }
    cout << "Server is listening on port " << proxyPort << endl << endl;

    // enters main loop which accepts incoming connections to the server
    while (true) {
        int childsockfd;
        // accepts the first client in the queue and creates a new file descriptor for that connection
        if ((childsockfd = accept(parentsockfd, (struct sockaddr *) &caddr, (socklen_t *) &sockSize)) < 0) {
            if (errno == 22) {
                cout << "Server has stopped accepting new connections" << endl;
                cout << "Program will close after 5 seconds" << endl;
                sleep(5);
                exit(0);
            }
            cout << "Error accepting: " << errno << endl;
            cout << "IP: " << inet_ntoa(caddr.sin_addr) << " - Port: " << ntohs(caddr.sin_port) << endl;
            exit(1);
        }
        cout << "Accepted connection from IP: " << inet_ntoa(caddr.sin_addr) << " - Port: " << ntohs(caddr.sin_port) << endl;
        // process the clients request on a separate thread
        std::thread request(connection, childsockfd);
        request.detach();
    }
}

bool filterBody(const string &message) {
    if (filterList.empty()) {
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

bool filterURL(const string &url) {
    if (filterList.empty()) {
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
    if (send(sockfd, message.c_str(), message.length(), 0) == -1) {
        close(sockfd);
        cout << "Error Sending Data" << endl;
        return;
    }
}

void displayMenu() {
    string menu = "Proxy Menu:\r\n"
                  "1 - Toggle URL Filter\r\n"
                  "2 - Toggle Body Filter\r\n"
                  "3 - Filter List & Status\r\n"
                  "4 - Add Keyword To Filter\r\n"
                  "5 - Remove Keyword From Filter\r\n"
                  "6 - Clear Filter\r\n"
                  "7 - Shutdown Server\r\n"
                  "8 - Close connection\r\n\r\n";
    sendMessage(adminsockfd, menu);
}

void displayFilter() {
    if (URL_Filter) {
        sendMessage(adminsockfd, "URL Filter Active\r\n\r\n");
    } else {
        sendMessage(adminsockfd, "URL Filter Inactive\r\n\r\n");
    }
    if (BodyFilter) {
        sendMessage(adminsockfd, "Body Filter Active\r\n\r\n");
    } else {
        sendMessage(adminsockfd, "Body Filter Inactive\r\n\r\n");
    }
    if (filterList.empty()) {
        sendMessage(adminsockfd, "Filter is Empty\r\n\r\n");
    } else {
        sprintf(buffer, "%d Keywords in Filter:\r\n", filterList.size());
        sendMessage(adminsockfd, buffer);
        for (int i = 0; i < filterList.size(); i++) {
            sprintf(buffer, "%d : %s\r\n", i + 1, filterList[i].c_str());
            sendMessage(adminsockfd, buffer);
        }
        sendMessage(adminsockfd, "\r\n");
    }
}

void addKeyword() {
    sendMessage(adminsockfd, "Input Keyword to Add: ");
    int len = recv(adminsockfd, buffer, sizeof(buffer), 0);
    if(len < 0){
        close(adminsockfd);
        cout << "Admin has left" << endl;
        menuFlag = false;
        return;
    } else if (len == 0){
        return;
    }
    buffer[len - 2] = '\0';
    sendMessage(adminsockfd, "\r\n\r\n");
    string keyword(buffer);
    cout << keyword.c_str() << " - has been added to the filter list " << endl;
    filterList.push_back(keyword);
}

void removeKeyword() {
    sendMessage(adminsockfd, "Input Index of Keyword to Remove: ");
    int len = recv(adminsockfd, buffer, sizeof(buffer), 0);
    if(len < 0){
        close(adminsockfd);
        cout << "Admin has left" << endl;
        menuFlag = false;
        return;
    } else if (len == 0){
        return;
    }
    buffer[len -2] = '\0';
    sendMessage(adminsockfd, "\r\n\r\n");
    int index = buffer[0] - '0' - 1;
    if (index < 0 || index >= filterList.size()) {
        sendMessage(adminsockfd, "Error: Index Does Not Exists. Please Try Again.\r\n\r\n");
        return;
    }
    cout << filterList[index].c_str() << " - has been removed to the filter list " << endl;
    filterList.erase(filterList.begin() + index);
}

void selectMenuOption() {
    sendMessage(adminsockfd, "Enter option number (For help input - 9): ");
    int len = recv(adminsockfd, buffer, sizeof(buffer), 0);
    if(len < 0){
        close(adminsockfd);
        cout << "Admin has left" << endl;
        menuFlag = false;
        return;
    } else if (len == 0){
        return;
    }
    buffer[len -2] = '\0';
    sendMessage(adminsockfd, "\r\n\r\n");
    char option = buffer[0];
    switch (option) {
        case '1':
            URL_Filter = !URL_Filter;
            if (URL_Filter) {
                sendMessage(adminsockfd, "URL Filter Activated\r\n\r\n");
                cout << "URL Filter Activated" << endl;
            } else {
                sendMessage(adminsockfd, "URL Filter Deactivated\r\n\r\n");
                cout << "URL Filter Deactivated" << endl;
            }
            break;
        case '2':
            BodyFilter = !BodyFilter;
            if (BodyFilter) {
                sendMessage(adminsockfd, "Body Filter Activated\r\n\r\n");
                cout << "Body Filter Activated" << endl;
            } else {
                sendMessage(adminsockfd, "Body Filter Deactivated\r\n\r\n");
                cout << "Body Filter Deactivated" << endl;
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
            sendMessage(adminsockfd, "Filter cleared\r\n\r\n");
            cout << "Filter cleared" << endl;
            break;
        case '7':
            menuFlag = false;
            sendMessage(adminsockfd, "Server is shutting down\r\n\r\n");
            close(parentsockfd);
            close(adminsockfd);
            break;
        case '8':
            menuFlag = false;
            sendMessage(adminsockfd, "Admin connection closed\r\n\r\n");
            close(adminsockfd);
            break;
        case '9':
            displayMenu();
            break;
        default:
            sendMessage(adminsockfd, "Your input is invalid. Please select one of the following options:\r\n\r\n");
            displayMenu();
    }
}

void startMenu() {
    displayMenu();
    while (menuFlag) {
        selectMenuOption();
    }
    cout << "Config menu thread has closed" << endl;
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

void HTTP_ResponseParser(const string &response, string &contentType, int &contentLength) {
    int start;
    int end;

    start = response.find("Content-Type:");
    if (start != std::string::npos) {
        start += 14;
        end = start;
        while (response[end] != '\r') {
            end++;
        }
        contentType = response.substr(start, end - start);
    }

    start = response.find("Content-Length:");
    if (start != std::string::npos) {
        start += 16;
        int end = start;
        while (response[end] != '\r') {
            end++;
        }
        contentLength = stoi(response.substr(start, end - start), NULL);
    }
}

void connection(const int childsockfd) {
    // Gets the clients request as a string
    string incomingMessage = receiveMessage(childsockfd);

    string method;
    string path;
    string version;
    string host;
    // Parses the request and filters the urls if url filter is active
    HTTP_RequestParser(incomingMessage, method, path, version, host);
    // If request contains bad content then client is redirected to error page else
    if (filterURL(path) && URL_Filter) {
        string error = "HTTP/1.0 301 Moved Permanently\r\nLocation: http://pages.cpsc.ucalgary.ca/~carey/CPSC441/ass1/error.html\r\n\r\n";
        sendMessage(childsockfd, error);
        close(childsockfd);
        return;
    }
    cout << path.c_str() << " requested from host " << host.c_str() << endl;

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

    int sockfd;
    // Creates a socket which is used to connect to the specified server
    if ((sockfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol)) == -1) {
        std::cout << "Error creating socket" << std::endl;
        std::cout << errno << std::endl;
        close(childsockfd);
        return;
    }
    // Connects to the requested server
    if (connect(sockfd, (struct sockaddr *) addr4, sockSize) == -1) {
        std::cout << "Cannot connect" << std::endl;
        std::cout << errno << std::endl;
        close(childsockfd);
        close(sockfd);
        return;
    }

    sendMessage(sockfd, incomingMessage);

    // Gets the response from the server
    string message = receiveMessage(sockfd);
    close(sockfd);

    string contentType;
    int contentLength;
    // Parses the response and filters the body if body filter is active
    HTTP_ResponseParser(message, contentType, contentLength);
    cout << "content length: " << contentLength << endl;
    cout << "content type: " << contentType.c_str() << endl;

    // If request contains bad content then client is redirected to error page else
    // Sends the response to the client and closes connections
    if (BodyFilter) {
        if (strncmp(contentType.c_str(), "text", 4) != 0) {
            sendMessage(childsockfd, message);
        } else if (filterBody(message) &&
                   strcmp(path.c_str(), "http://pages.cpsc.ucalgary.ca/~carey/CPSC441/ass1/error.html") != 0) {
            string error = "HTTP/1.0 301 Moved Permanently\r\nLocation: http://pages.cpsc.ucalgary.ca/~carey/CPSC441/ass1/error.html\r\n\r\n";
            sendMessage(childsockfd, error);
        } else {
            sendMessage(childsockfd, message);
        }
    } else {
        sendMessage(childsockfd, message);
    }
    close(childsockfd);
}