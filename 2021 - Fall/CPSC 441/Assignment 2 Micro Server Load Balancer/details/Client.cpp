#include <iostream>     // For cerr and cout
#include <cstdlib>      // For atoi()
#include <sys/types.h>  // For data types
#include <sys/socket.h> // For socket(), connect(), send(), and recv()
#include <netdb.h>      // For gethostbyname()
#include <arpa/inet.h>  // For inet_addr()
#include <unistd.h>     // For close()
#include <netinet/in.h> // For sockaddr_in
#include <string.h>     // memset, strcat, ...

using namespace std;

// Variables
#define BUFSIZE 256 // Buffer Size

// Functions
void echo(int client_socket);                   // to handle echo requests
void handle_translate(int client_socket);       // to handle translate request
void hand_currency_exchange(int client_socket); // to handle currency exchange request
void handle_vote(int client_socket);            // to handle voting request
void handle_result(int client_socket);          // to show result if client has voted
void show_message(int out, string message);     // to show message and exit() in case of a system call failuire

/* 
    Client's Main function
    It initiates a TCP connection with the Indirection Server.
    Client is aware of the indirectional server's IP address and Port number.
    It receives the server's ip and port number by command line
 */
int main(int argc, char *argv[])
{
    int client_sockfd; // client socket

    // check the number of command line arguments
    if (argc != 3)
    {
        cerr << "Usage: " << argv[0] << "<Server IP> <Server Port>" << endl;
        exit(1);
    }

    // use the addrinfo struct to identify an Internet host
    addrinfo hints, *client_info;

    memset(&hints, 0, sizeof(addrinfo));
    hints.ai_family = AF_INET;       // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP

    // get the Server's info
    int output_check = getaddrinfo(argv[1], argv[2], &hints, &client_info); // https://man7.org/linux/man-pages/man3/getaddrinfo.3.html
    if (output_check != 0)                                                  // getaddrinfo returns 0 if it succeeds
        show_message(-1, "getaddrinfo() failed!");

    // create a socket
    client_sockfd = socket(client_info->ai_family, client_info->ai_socktype, client_info->ai_protocol); // On success, a file descriptor for the new socket is returned.  On error, -1 is returned.
    show_message(client_sockfd, "socket() failed.");

    // connect
    output_check = connect(client_sockfd, client_info->ai_addr, client_info->ai_addrlen); // If the connection or binding succeeds, zero is returned. On error, -1 is returned.

    show_message(output_check, "connect() failed.");

    printf("\nYou are connected now.\n\n");

    while (1) // 0 for exit inside the loop
    {
        char buffer[BUFSIZE]; // client buffer
        memset(buffer, 0, sizeof(buffer));

        strcpy(buffer, "Hi"); // send a message "Hi" to the server
        size_t sent_size = send(client_sockfd, buffer, sizeof(buffer), 0);
        show_message(sent_size, "sent() failed");
        printf("\nClient: (%ld bytes): %s\n", sent_size, buffer);

        // recv from the server
        memset(buffer, 0, sizeof(buffer));
        size_t rcv_size = recv(client_sockfd, buffer, sizeof(buffer), 0);
        show_message(rcv_size, "recv() failed!");
        printf("Received Message (%ld bytes): \n\n%s\n", rcv_size, buffer);

        // get user's command
        memset(buffer, 0, sizeof(buffer));
        printf("\nCommand: ");
        std::cin >> buffer;

        // send use's comamnd to the server
        send(client_sockfd, buffer, sizeof(buffer), 0);

        system("clear"); // cleae the screen

        // receive next step from server
        int my_choose = atoi(buffer);
        memset(buffer, 0, sizeof(buffer));
        rcv_size = recv(client_sockfd, buffer, sizeof(buffer), 0);
        printf("Received Message (%ld bytes): %s\n", rcv_size, buffer);

        // handle the user's command
        switch (my_choose)
        {
        case 0:             // exit
            goto exit_loop; // go to the exit_loop label
            break;
        case 1: // translate
            handle_translate(client_sockfd);
            break;
        case 2: // currency exchange
            hand_currency_exchange(client_sockfd);
            break;
        case 3: // vote a candidate
            handle_vote(client_sockfd);
            break;
        case 4: // see the voting result
            handle_result(client_sockfd);
            break;
        case 9: // send an echo message
            echo(client_sockfd);
            break;

        default: // something else
            printf("This command is not in the list. \n");
            break;
        }
    }
exit_loop:;
    close(client_sockfd); // close the socket
    printf("Connection closed!\n");
    return 0;
}

// it checks if the return value from a function call is negative, it shows the "message" and exit
void show_message(int out, string message)
{
    if (out < 0)
    {
        cout << message << endl;
        exit(0);
    }
}

// Simple echo program
void echo(int client_socket)
{
    char buffer[BUFSIZE];
    // go into a loop to send and receive echo messages
    // Enter 0 for exit from the echo program and return to the main page
    while (1)
    {
        memset(buffer, 0, sizeof(buffer));
        // get message from a user
        printf("Enter a message (0 for exit): ");
        cin >> buffer;

        // send message to the indirection server
        size_t sent_size = send(client_socket, buffer, sizeof(buffer), 0);

        show_message(sent_size, "send() failed");
        printf("Sent (%ld bytes): %s\n", sent_size, buffer);

        // break if user's input is 0
        if (!strcmp(buffer, "0"))
            break;

        // recv from the indirection server
        memset(buffer, 0, sizeof(buffer));
        size_t recv_size = recv(client_socket, buffer, sizeof(buffer), 0);
        printf("Received (%ld bytes): %s\n", recv_size, buffer);
    }
}

/*
This function get an English word, and sent it the the indirection server to be translated to the French
it only accept one word as the input
try: Hello, Boy, Girl
0 to exit   
 */
void handle_translate(int client_socket)
{
    char buffer[BUFSIZE];

    // go into a loop to send and receive echo messages
    memset(buffer, 0, sizeof(buffer));
    // get message from a user
    printf("Enter an English word (0 to exit): ");
    cin >> buffer;

    // send message to the indirection server
    size_t sent_size = send(client_socket, buffer, sizeof(buffer), 0);

    show_message(sent_size, "send() failed");
    printf("Sent (%ld bytes): %s\n", sent_size, buffer);
    if (!strcmp(buffer, "0"))
        return;

    // recv from the indirection server
    memset(buffer, 0, sizeof(buffer));
    size_t recv_size = recv(client_socket, buffer, sizeof(buffer), 0);
    printf("Received (%ld bytes): %s\n", recv_size, buffer);
}
/*
It received a set of three values in this order
<src currency> <dst currency> <money value in src currency>
for example: CAD USD 100
 */
void hand_currency_exchange(int client_socket)
{
    char buffer[BUFSIZE];
    // go into a loop to send and receive echo messages

    memset(buffer, 0, sizeof(buffer));
    // get src currecny, dst currency, and value from the user input
    printf("Enter <SRC> <DST> <VALUE>: ");
    char src[4], dst[4], value[10];
    cin >> src >> dst >> value;
    strcat(buffer, src);
    strcat(buffer, " ");
    strcat(buffer, dst);
    strcat(buffer, " ");
    strcat(buffer, value);

    // send message to the indirection server
    size_t sent_size = send(client_socket, buffer, sizeof(buffer), 0);

    show_message(sent_size, "send() failed");
    printf("Sent (%ld bytes): %s\n", sent_size, buffer);
    if (!strcmp(buffer, "0"))
        return;

    // recv from the indirection server
    memset(buffer, 0, sizeof(buffer));
    size_t recv_size = recv(client_socket, buffer, sizeof(buffer), 0);
    printf("Received (%ld bytes): %s\n", recv_size, buffer);
}
/*
It handle communication to vote and receive voting table 
 */
void handle_vote(int client_socket)
{
    char buffer[BUFSIZE];

    // Recv <-- indirection: Key
    // recv the voting key from the indirection server
    memset(buffer, 0, sizeof(buffer));
    size_t recv_size = recv(client_socket, buffer, sizeof(buffer), 0);
    printf("Received (%ld bytes): %s (KEY)\n", recv_size, buffer);

    // prevent duplicate voting
    if (!strcmp(buffer, "NO"))
    {
        printf("You have voted already.\n");
        return;
    }

    int key;
    key = atoi(buffer);

    // Send --> indirection: send LIST messgae to get the candidate list
    memset(buffer, 0, sizeof(buffer));
    strcpy(buffer, string("LIST").c_str());
    size_t sent_size = send(client_socket, buffer, sizeof(buffer), 0);
    show_message(sent_size, "send() failed");
    printf("Sent (%ld bytes): %s (LIST)\n", sent_size, buffer);
    // send message to the indirection server

    // Recv <-- indirection: LIST
    // recv from the indirection server
    memset(buffer, 0, sizeof(buffer));
    recv_size = recv(client_socket, buffer, sizeof(buffer), 0);
    printf("Received (%ld bytes): %s (LIST)\n", recv_size, buffer);

    // Send --> indirection: Get user vote, encrypt it, and sent it to the indirection server
    memset(buffer, 0, sizeof(buffer));
    printf("Vote by enterring the Candidate's ID.\nYour Vote: ");

    int user_vote;
    cin >> user_vote;
    user_vote *= key;
    memset(buffer, 0, sizeof(buffer));
    strcpy(buffer, to_string(user_vote).c_str());

    // send message to the indirection server
    sent_size = send(client_socket, buffer, sizeof(buffer), 0);
    show_message(sent_size, "send() failed");
    printf("Sent (%ld bytes): %s (Encrypted VOTE)\n", sent_size, buffer);

    // Rcv --> indirection: get thank youe message from
    memset(buffer, 0, sizeof(buffer));
    recv_size = recv(client_socket, buffer, sizeof(buffer), 0);
    printf("Received (%ld bytes): %s\n", recv_size, buffer);
    memset(buffer, 0, sizeof(buffer));
}

/*
    It handle communication to vote and receive voting table 
 */
void handle_result(int client_socket)
{
    char buffer[BUFSIZE];

    // Recv <-- indirection: Key
    // recv the voting key from the indirection server
    memset(buffer, 0, sizeof(buffer));
    size_t recv_size = recv(client_socket, buffer, sizeof(buffer), 0);
    // prevent duplicate voting
    if (!strcmp(buffer, "NO"))
    {
        printf("You have not voted yet.\n");
        return;
    }
    printf("Received (%ld bytes): %s (RESULT)\n", recv_size, buffer);
}