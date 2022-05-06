#include <cstdlib>      // For atoi()
#include <sys/types.h>  // For data types
#include <sys/socket.h> // For socket(), connect(), send(), and recv()
#include <netdb.h>      // For gethostbyname()
#include <arpa/inet.h>  // For inet_addr()
#include <unistd.h>     // For close()
#include <netinet/in.h> // For sockaddr_in
#include <iostream>     // For cerr and cout
#include <cstring>      // string parsing
#include <vector>       // vector data type
#include <time.h>       // timeval
using namespace std;

// Micro servers IPs and Port numbers
#define TRANSLATE_IP "136.159.5.27"
#define TRANSLATE_PORT "1111"
#define CURRENCY_EXCHANGE_IP "136.159.5.27"
#define CURRENCY_EXCHANGE_PORT "2222"
#define VOTE_IP "136.159.5.27"
#define VOTE_PORT "3333"
#define ECHO_IP "136.159.5.27"
#define ECHO_PORT "9999"

#define BUFSIZE 256

// Functions
void handle_client_request(int client_socket, addrinfo server_info); // handle clients connection
void print_buffer(string action, char buffer[], size_t bytes);       // print sent/recv messages
void translate(int client_socket);                                   // Eng to Fr translation service
void currency_exchange(int client_socket);                           // handle a currecny exchange service
void vote(int client_socket, addrinfo *server_info);                 // address client's voting request
void show_results(int client_socket, addrinfo *server_info);         // address client's voting request
int echo(int client_socket);                                         // transfer messages between a client and a echo micro server
void show_message(int out, string message);                          // handle potential system call fails
size_t send_to_micro(int micro_socket, char *buffer, sockaddr ai_address, socklen_t ai_address_len);
size_t recv_from_micro(int client_socket, char *buffer, sockaddr ai_address, socklen_t ai_address_len);
size_t send_to_client(int client_socket, char *buffer);
size_t recv_from_client(int client_socket, char *buffer);

/*
The main function
 */
int main(int argc, const char *argv[])
{
    // Variables
    int server_sockfd, client_sockfd; // server socket
    int check_output;                 // keep the system calls' return value

    char server_port_number[5]; // for default port number
    memset(server_port_number, 0, sizeof(server_port_number));

    // make sure server receive the program
    if (argc != 2)
    {
        cerr << "Using default port number (8888)\n";
        strcpy(server_port_number, "8888");
    }
    else
    {
        strcpy(server_port_number, argv[0]);
    }

    // set up a TCP server socket
    addrinfo hints, *server_info;
    memset(&hints, 0, sizeof(addrinfo));
    hints.ai_family = AF_INET;       // IPv4
    hints.ai_flags = AI_PASSIVE;     // Use my cuttent IP address
    hints.ai_socktype = SOCK_STREAM; // TCP

    // get server address info
    check_output = getaddrinfo(NULL, argv[1], &hints, &server_info);

    // a succesfull getaddrinfo()'s return value is 0
    if (check_output != 0)
        show_message(-1, "getaddrinfo() failed");

    // Create a socket
    server_sockfd = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol); //  create an endpoint for communication
    //  On success, zero is returned.  On error, -1 is returned
    show_message(server_sockfd, "socket() failed.");

    // bind to the port
    check_output = bind(server_sockfd, server_info->ai_addr, server_info->ai_addrlen);
    // On success, zero is returned.  On error, -1 is returned
    show_message(check_output, "bind() failed()");

    // listen
    check_output = listen(server_sockfd, 5);
    // On success, zero is returned.  On error, -1 is returned
    show_message(check_output, "listen failed");

    printf("Server is listining ... \n");

    // sockaddr_storage their_addr;
    // socklen_t sin_size = sizeof(their_addr);
    client_sockfd = accept(server_sockfd, server_info->ai_addr, &(server_info->ai_addrlen));

    show_message(client_sockfd, "accept() failed");

    handle_client_request(client_sockfd, *server_info);

    return 0;
}

/*
    after a user connected, it handles the user's commands
 */
void handle_client_request(int client_socket, addrinfo server_info)
{
    printf("Client Connected.\n");

    while (1)
    {
        char buffer[BUFSIZE] = "";
        memset(buffer, 0, sizeof(buffer));
        // Receive from clinet
        size_t rcv_size = recv_from_client(client_socket, buffer);
        show_message(rcv_size, "recv_from() failed");

        // send the action list to the client
        memset(buffer, 0, sizeof(buffer));
        strcpy(buffer, "Select an action: \n1. Translate\n2. Currency Exchange\n3. Vote\n4. View Voting Result\n9. Echo\n0. Close Connection");
        size_t sent_size = send_to_client(client_socket, buffer);
        show_message(sent_size, "send_to_client() failed");

        // receive client's action
        memset(buffer, 0, sizeof(buffer));

        rcv_size = recv_from_client(client_socket, buffer);

        int user_req = atoi(buffer);

        // confirm receiving the user input
        memset(buffer, 0, sizeof(buffer));
        strcpy(buffer, "OK");
        sent_size = send_to_client(client_socket, buffer);

        // switch user action
        switch (user_req)
        {
        case 0:
            // exit
            goto endloop;
            break;
        case 1:
            translate(client_socket);
            break;
        case 2:
            currency_exchange(client_socket);
            break;
        case 3:
            vote(client_socket, &server_info);
            break;
        case 4:
            show_results(client_socket, &server_info);
            break;
        case 9:
        {
            echo(client_socket);
            break;
        }
        default:
            printf("Please select a correct number. \n");
            break;
        }
    }
endloop:;
    close(client_socket);
    printf("Connection closed.\n");
}

void translate(int client_socket)
{
    // buffer spcae to send and receive data
    char buffer[BUFSIZE];
    memset(buffer, 0, sizeof(buffer));

    // receive message from the client
    size_t recv_size = recv_from_client(client_socket, buffer);
    show_message(recv_size, "recv_from_client() failed");
    if (!strcmp(buffer, "0"))
        return;

    // send it to the echo server
    printf("Connecting to the micro server ... \n");
    // get micro server info
    addrinfo hints, *micro_info;
    memset(&hints, 0, sizeof(addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    int checking = getaddrinfo(TRANSLATE_IP, TRANSLATE_PORT, &hints, &micro_info);
    if (checking != 0)
        show_message(-1, "getaddrinfo() failed");

    // create a socket
    int micro_socket = socket(micro_info->ai_family, micro_info->ai_socktype, micro_info->ai_protocol);
    // cout << micro_socket << endl;
    show_message(micro_socket, "socket() failed");

    // send to the micro-server
    size_t sent_size = send_to_micro(micro_socket, buffer, *micro_info->ai_addr, micro_info->ai_addrlen);
    show_message(sent_size, "send_to_micro() failed");

    // receive data from micro server
    memset(buffer, 0, sizeof(buffer));
    recv_size = recv_from_micro(micro_socket, buffer, *micro_info->ai_addr, micro_info->ai_addrlen);

    close(micro_socket);

    // send to the client
    sent_size = send_to_client(client_socket, buffer);
    memset(buffer, 0, sizeof(buffer));
}

// simple function to print buffer content and size
void print_buffer(string action, char buffer[], size_t bytes)
{
    printf("%s: %s (%ld bytes)\n", action.c_str(), buffer, bytes);
}

void vote(int client_socket, addrinfo *server_info)
{
    // get clients IP address and attach it to the message
    sockaddr_in *address = (sockaddr_in *)server_info->ai_addr;
    char ip_str[INET_ADDRSTRLEN];
    inet_ntop(server_info->ai_family, &(address->sin_addr), ip_str, INET_ADDRSTRLEN);
    cout << "Client IP Address: " << ip_str << endl;

    // create a socket to connet to the voting micro server
    printf("Connecting to the micro server ... \n");
    // get micro server info
    addrinfo hints, *micro_info;
    memset(&hints, 0, sizeof(addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    int checking = getaddrinfo(VOTE_IP, VOTE_PORT, &hints, &micro_info);
    if (checking != 0)
        show_message(-1, "getaddrinfo() failed");

    // create a socket
    int micro_socket = socket(micro_info->ai_family, micro_info->ai_socktype, micro_info->ai_protocol);
    // cout << micro_socket << endl;
    show_message(micro_socket, "socket() failed");

    // Send KEY command + client's ip address
    char buffer[BUFSIZE];
    memset(buffer, 0, sizeof(buffer));
    strcat(buffer, "KEY");
    strcat(buffer, string(" ").c_str());
    strcat(buffer, ip_str);
    size_t send_size = send_to_micro(micro_socket, buffer, *micro_info->ai_addr, micro_info->ai_addrlen);

    // Recv <-- micro: Key
    size_t recv_size = recv_from_micro(micro_socket, buffer, *micro_info->ai_addr, micro_info->ai_addrlen);

    // send to the client
    send_size = send_to_client(client_socket, buffer);
    // if micro server is offline, recv_from_micro() returns -1
    if (recv_size == -1)
    {
        close(micro_socket);
        return;
    }

    // handle duplicate voting
    if (!strcmp(buffer, "NO")) // if micro sends NO, it means this client has voted before
    {
        close(micro_socket);
        return;
    }

    // Rcv --> Client: List request
    recv_size = recv_from_client(client_socket, buffer);

    // Send --> Micro: List
    send_size = send_to_micro(micro_socket, buffer, *micro_info->ai_addr, micro_info->ai_addrlen);

    // Rcv --> Micro: List
    recv_size = recv_from_micro(micro_socket, buffer, *micro_info->ai_addr, micro_info->ai_addrlen);

    // Send --> Client: List
    send_size = send_to_client(client_socket, buffer);

    // Rcv --> Client: Vote
    recv_size = recv_from_client(client_socket, buffer);

    // Send --> Micro: Vote
    // get clients IP address and attach it to the message
    strcat(buffer, string(" ").c_str());
    strcat(buffer, "VOTE");
    strcat(buffer, string(" ").c_str());
    strcat(buffer, ip_str);
    send_size = send_to_micro(micro_socket, buffer, *micro_info->ai_addr, micro_info->ai_addrlen);

    // Rcv --> Micro: Update
    recv_size = recv_from_micro(micro_socket, buffer, *micro_info->ai_addr, micro_info->ai_addrlen);
    // Send --> Client: Done
    send_size = send_to_client(client_socket, buffer);
    // close socket to the micro server
    close(micro_socket);
}

void show_results(int client_socket, addrinfo *server_info)
{
    // get clients IP address and attach it to the message
    sockaddr_in *address = (sockaddr_in *)server_info->ai_addr;
    char ip_str[INET_ADDRSTRLEN];
    inet_ntop(server_info->ai_family, &(address->sin_addr), ip_str, INET_ADDRSTRLEN);
    cout << "Client IP Address: " << ip_str << endl;

    // create a socket to connet to the voting micro server
    printf("Connecting to the micro server ... \n");
    // get micro server info
    addrinfo hints, *micro_info;
    memset(&hints, 0, sizeof(addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    int checking = getaddrinfo(VOTE_IP, VOTE_PORT, &hints, &micro_info);
    if (checking != 0)
        show_message(-1, "getaddrinfo() failed");

    // create a socket
    int micro_socket = socket(micro_info->ai_family, micro_info->ai_socktype, micro_info->ai_protocol);
    // cout << micro_socket << endl;
    show_message(micro_socket, "socket() failed");
    // Send Result command + client's ip address
    char buffer[BUFSIZE];
    memset(buffer, 0, sizeof(buffer));
    strcat(buffer, "RESULT");
    strcat(buffer, string(" ").c_str());
    strcat(buffer, ip_str);

    size_t send_size = send_to_micro(micro_socket, buffer, *micro_info->ai_addr, micro_info->ai_addrlen);

    // Recv <-- micro: RESULT
    size_t recv_size = recv_from_micro(micro_socket, buffer, *micro_info->ai_addr, micro_info->ai_addrlen);

    // send to the client
    send_size = send_to_client(client_socket, buffer);

    close(micro_socket);
}

int echo(int client_socket)
{
    // buffer spcae to send and receive data
    char buffer[BUFSIZE];

    // go to a loop to receive and echo
    while (1)
    {
        // receive message from the client
        size_t recv_size = recv_from_client(client_socket, buffer);
        if (!strcmp(buffer, "0"))
            break;

        // create a udp socket
        printf("Connecting to the micro server ... \n");
        // get micro server info
        addrinfo hints, *micro_info;
        memset(&hints, 0, sizeof(addrinfo));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_DGRAM;
        int checking = getaddrinfo(ECHO_IP, ECHO_PORT, &hints, &micro_info);
        if (checking != 0)
            show_message(-1, "getaddrinfo() failed");

        int micro_socket = socket(micro_info->ai_family, micro_info->ai_socktype, micro_info->ai_protocol);
        show_message(micro_socket, "socket() failed");

        // send to the micro
        size_t send_size = send_to_micro(micro_socket, buffer, *micro_info->ai_addr, micro_info->ai_addrlen);

        // receive data from micro server
        recv_size = recv_from_micro(micro_socket, buffer, *micro_info->ai_addr, micro_info->ai_addrlen);
        // printf("\n\nsend_to() failed!!!\n\n");
        if (send_size <= 0)
        {
            printf("send_to() failed!!!\n\n");
            break;
        }
        // close the micro connection
        close(micro_socket);

        // send to the client
        send_size = send_to_client(client_socket, buffer);
        memset(buffer, 0, sizeof(buffer));
    }
    return 1;
}

void currency_exchange(int client_socket)
{
    // buffer spcae to send and receive data
    char buffer[BUFSIZE];

    // receive message from the client
    size_t recv_size = recv_from_client(client_socket, buffer);
    if (!strcmp(buffer, "0"))
        return;

    printf("Connecting to the micro server ... \n");
    // get micro server info
    addrinfo hints, *micro_info;
    memset(&hints, 0, sizeof(addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    int checking = getaddrinfo(CURRENCY_EXCHANGE_IP, CURRENCY_EXCHANGE_PORT, &hints, &micro_info);
    if (checking != 0)
        show_message(-1, "getaddrinfo() failed");
    // create a socket
    int micro_socket = socket(micro_info->ai_family, micro_info->ai_socktype, micro_info->ai_protocol);
    // cout << micro_socket << endl;
    show_message(micro_socket, "socket() failed");
    size_t send_size = send_to_micro(micro_socket, buffer, *micro_info->ai_addr, micro_info->ai_addrlen);

    // receive data from micro server
    recv_size = recv_from_micro(micro_socket, buffer, *micro_info->ai_addr, micro_info->ai_addrlen);

    // close the micro connection
    close(micro_socket);

    // send to the client
    send_size = send_to_client(client_socket, buffer);
    memset(buffer, 0, sizeof(buffer));
}

// send to from indirection server to the micro servers
size_t send_to_micro(int micro_socket, char client_buffer[], const sockaddr ai_address, socklen_t ai_address_len)
{
    char buffer[BUFSIZE];
    memset(buffer, 0, sizeof(buffer));
    strcpy(buffer, client_buffer);
    // https://linux.die.net/man/2/sendto

    size_t send_size = sendto(micro_socket, buffer, strlen(buffer), MSG_DONTWAIT, &ai_address, ai_address_len);

    // On success, these calls return the number of characters sent. On error, -1 is returned, and errno is set appropriately.
    show_message(send_size, "sentto() failed");
    print_buffer("Sent to Micro", buffer, send_size);
    // memset(client_buffer, 0, sizeof(client_buffer));
    // strcpy(client_buffer, buffer);
    return send_size;
}

size_t recv_from_micro(int micro_socket, char client_buffer[], sockaddr ai_address, socklen_t ai_address_len)
{
    char buffer[BUFSIZE];
    memset(buffer, 0, sizeof(buffer));

    // timeout
    // source: http://www.mathcs.emory.edu/~cheung/Courses/455/Syllabus/9-netw-prog/timeout.html
    timeval micro_timeout;     // time value for time out
    fd_set fds_read, fds_main; // fd's used by select

    // Set up the timeout value
    micro_timeout.tv_sec = 3; /*set the timeout to 10 seconds*/
    micro_timeout.tv_usec = 0;
    size_t recv_size;
    // set up he descriptor set for select()
    FD_ZERO(&fds_main);              // clear fds
    FD_SET(micro_socket, &fds_main); // set fds to the micro socket

    // wait for recvfrom() until timeout expires
    if (select(32, &fds_main, NULL, NULL, &micro_timeout) == 0)
    {
        printf("recvfrom() timed out\n");
        memset(client_buffer, 0, strlen(client_buffer));
        strcpy(client_buffer, "Micro Server is offline.");
        return (-1);
    }
    else
    {
        recv_size = recvfrom(micro_socket, buffer, sizeof(buffer), 0, &ai_address, &ai_address_len);
        show_message(recv_size, "recvfrom() failed");
        print_buffer("Receive from Micro: ", buffer, recv_size);
        memset(client_buffer, 0, strlen(client_buffer));
        strcpy(client_buffer, buffer);
        return (recv_size);
    }
}

// send to the client
size_t send_to_client(int client_socket, char client_buffer[])
{
    char buffer[BUFSIZE];
    memset(buffer, 0, sizeof(buffer));
    strcpy(buffer, client_buffer);
    size_t send_size = send(client_socket, buffer, sizeof(buffer), 0);
    show_message(send_size, "sent() failed");
    print_buffer("Sent to client", buffer, send_size);
    memset(client_buffer, 0, strlen(client_buffer));
    strcpy(client_buffer, buffer);
    return send_size;
}

// Rcv --> Client
// https://man7.org/linux/man-pages/man2/recv.2.html
size_t recv_from_client(int client_socket, char client_buffer[])
{
    char buffer[BUFSIZE];
    memset(buffer, 0, sizeof(buffer));
    size_t recv_size = recv(client_socket, buffer, sizeof(buffer), 0);
    show_message(recv_size, "recv() failed");
    print_buffer("Receive from Client", buffer, recv_size);
    memset(client_buffer, 0, strlen(client_buffer));
    strcpy(client_buffer, buffer);
    return recv_size;
}

// check the output of a call, and if it is negative, show the message and exit
void show_message(int out, string message)
{
    if (out < 0)
    {
        cout << message << endl;
        exit(0);
    }
}
