#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

void getCombination(char *arr, int num) {
    int i = 0;
    while (num >= 0) {
        arr[i] = num % 26;
        num -= arr[i];
        arr[i] += 65;
        i++;
        num = num / 26 - 1;
    }
    arr[i] = '\0';
}

int main() {
//    struct in_addr {
//        unsigned long s_addr;          // load with inet_pton()
//    };
//
//    struct sockaddr_in {
//        short            sin_family;   // e.g. AF_INET, AF_INET6
//        unsigned short   sin_port;     // e.g. htons(3490)
//        struct in_addr   sin_addr;     // see struct in_addr, below
//        char             sin_zero[8];  // zero this if you want to
//    };
//
//    struct sockaddr {
//        unsigned short    sa_family;    // address family, AF_xxx
//        char              sa_data[14];  // 14 bytes of protocol address
//    };

    struct sockaddr_in saddr;

    char buffer[128];


    // unsigned short port = 44101;
     unsigned short port = 800;
    unsigned short port2 = 44102;
    // const char *address = "136.159.5.25";
    const char *address = "127.0.0.1";
    int sockfd;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        std::cout << "Error creating socket" << std::endl;
        std::cout << errno << std::endl;
        exit(1);
    }
    // memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET; // match the socket() call
    saddr.sin_port = htons(port); // specify port to connect to
    saddr.sin_addr.s_addr = inet_addr(address);
//    if(inet_pton(AF_INET, "136.159.5.25", &saddr.sin_addr) == -1){
//        printf("\nInvalid address/ Address not supported \n");
//        return -1;
//    }
    // std::cout << saddr.sin_addr.s_addr << std::endl;
    // saddr.sin_addr.s_addr = 2292122905;
    int connfd;
    if ((connfd = connect(sockfd, (struct sockaddr *) &saddr, sizeof(saddr))) == -1) { // connect!
        std::cout << "Cannot connect" << std::endl;
        std::cout << errno << std::endl;
    }

//    for (int i = 0; i <= 1; i++) {
//        getCombination(buffer, i);
        // std::cout << i << " : " << buffer << std::endl;
//        strcpy(buffer, "FUN");
//        strcpy(buffer, "COUNT");
while(true){
    std::cout << "Enter Command" << std::endl;
    std::cin >> buffer;
    if(strcmp("BYE", buffer) == 0){
        break;
    }
        send(sockfd, buffer, strlen(buffer), 0);
        int len = recv(sockfd, buffer, 128, 0);
        buffer[len - 1] = '\0';
    std::cout << buffer << std::endl;
//        if (strcmp("Unrecognized command. Sorry!", buffer) != 0) {
//            std::cout << buffer << std::endl;
//        }
//        if (strcmp("Command limit reached. So tired. Goodbye!", buffer) == 0) {
//            // std::cout << "buffer" << std::endl;
//            close(sockfd);
//            if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
//                std::cout << "Error creating socket" << std::endl;
//                std::cout << errno << std::endl;
//            }
//            if ((connfd = connect(sockfd, (struct sockaddr *) &saddr, sizeof(saddr))) == -1) { // connect!
//                std::cout << "Cannot connect" << std::endl;
//                std::cout << errno << std::endl;
//            }
//        }
    }
    close(sockfd);
    return 0;
}
