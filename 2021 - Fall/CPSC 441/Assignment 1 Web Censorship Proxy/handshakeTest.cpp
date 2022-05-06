#include <iostream>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <openssl/ssl.h>

using namespace std;

int main () {
    char buffer[1024];
    int sockSize = sizeof(struct sockaddr_in);
    struct addrinfo hints, *result;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    if(getaddrinfo("www.microsoft.com", NULL, &hints, &result) != 0){
        cout << "address info error" << endl;
        exit(0);
    }
    struct sockaddr_in *addr4 = (struct sockaddr_in *) result->ai_addr;
    inet_ntop(AF_INET, &addr4->sin_addr, buffer, sizeof(buffer));
    addr4->sin_port = htons(443);
    int sockfd;
    if ((sockfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol)) == -1) {
        std::cout << "Error creating socket: " << errno << std::endl;
        exit(1);
    }
    int connfd;
    if ((connfd = connect(sockfd, (struct sockaddr *) addr4, sockSize)) == -1) {
        std::cout << "Cannot connect: " << errno << std::endl;
        exit(1);
    }
    strcpy(buffer, "CONNECT www.microsoft.com:443 HTTP/1.0");
    send(sockfd, buffer, strlen(buffer), 0);
    recv(sockfd, buffer, sizeof(buffer), 0);
    cout << buffer;
    close(sockfd);
    return 0;
}