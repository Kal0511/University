#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

int main() {
    struct sockaddr_in saddr;
    char buffer[128];
    unsigned short port = 44101;
    unsigned short port2 = 44102;
    const char *address = "136.159.5.25";
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    saddr.sin_addr.s_addr = inet_addr(address);

    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        std::cout << "Error creating socket" << std::endl;
        std::cout << errno << std::endl;
        exit(1);
    }

    int connfd;
    if ((connfd = connect(sockfd, (struct sockaddr *) &saddr, sizeof(saddr))) == -1) {
        std::cout << "Cannot connect" << std::endl;
        std::cout << errno << std::endl;
    }

    while (true) {
        std::cout << "Enter Command" << std::endl;
        std::cin >> buffer;
        if (strcmp("BYE", buffer) == 0) {
            break;
        }
        send(sockfd, buffer, strlen(buffer), 0);
        int len = recv(sockfd, buffer, 128, 0);
        buffer[len - 1] = '\0';
        std::cout << buffer << std::endl;
    }

    close(sockfd);
    return 0;
}
