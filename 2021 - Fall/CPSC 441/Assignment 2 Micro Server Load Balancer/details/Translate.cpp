#include <iostream>     // For cerr and cout
#include <cstdlib>      // For atoi()
#include <sys/types.h>  // For data types
#include <sys/socket.h> // For socket(), connect(), send(), and recv()
#include <netdb.h>      // For gethostbyname()
#include <arpa/inet.h>  // For inet_addr()
#include <unistd.h>     // For close()
#include <netinet/in.h> // For sockaddr_in
#include <cstring>
#include <vector>

#define MAIN_SERVER_IP "136.159.5.25"
#define MAIN_SERVER_PORT "1111"
#define BUFSIZE 100

using namespace std;

// functions
void fill_table(vector<pair<const char *, const char *>> &my_dictionary);
void print_table(vector<pair<const char *, const char *>> my_dictionary);
const char *search_into_dictionary(vector<pair<const char *, const char *>> my_dictionary, const char *eng_word);
void show_message(int out, string message);
void print_buffer(string action, char buffer[], size_t bytes);

/* 
Main function
 */
int main(int argc, char const *argv[])
{
    vector<pair<const char *, const char *>> my_dictionary;
    fill_table(my_dictionary);

    addrinfo hints;
    addrinfo *micro_info;

    char buffer[BUFSIZE];

    memset(&hints, 0, sizeof(addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // for wildcard IP address
    hints.ai_protocol = 0;       // any protocol

    int checking = getaddrinfo(NULL, argv[1], &hints, &micro_info);

    show_message(checking, "getaddrinfo() failed!");

    // create a socket
    int micro_socket = socket(micro_info->ai_family, micro_info->ai_socktype, micro_info->ai_protocol);
    show_message(micro_socket, "socket() failed");

    // bing to the socket
    checking = bind(micro_socket, micro_info->ai_addr, micro_info->ai_addrlen);
    show_message(checking, "bind() failed");

    freeaddrinfo(micro_info);
    print_table(my_dictionary);

    for (;;)
    {
        // receive from main server
        memset(buffer, 0, sizeof(buffer));
        size_t recv_size = recvfrom(micro_socket, buffer, sizeof(buffer), 0, micro_info->ai_addr, &micro_info->ai_addrlen);
        print_buffer("Received", buffer, recv_size);

        strcpy(buffer, search_into_dictionary(my_dictionary, buffer));

        size_t send_size = sendto(micro_socket, buffer, sizeof(buffer), 0, micro_info->ai_addr, micro_info->ai_addrlen);
        show_message(send_size, "send() failed");
        print_buffer("Sent", buffer, send_size);
    }
}
// Fill Dictionary - Call by reference
void fill_table(vector<pair<const char *, const char *>> &my_dictionary)
{
    my_dictionary.push_back(make_pair("Hello", "Bonjour"));
    my_dictionary.push_back(make_pair("Girl", "Fille"));
    my_dictionary.push_back(make_pair("Boy", "Gar??on"));
    my_dictionary.push_back(make_pair("Dog", "Chien"));
    my_dictionary.push_back(make_pair("Cat", "Chat"));
    my_dictionary.push_back(make_pair("Goodbye", "Au revoir"));
}

void print_table(vector<pair<const char *, const char *>> my_dictionary)
{
    printf("\n\tWord List\n");
    for (int i = 0; i < my_dictionary.size(); i++)
    {
        printf("Eng: %-10s = Fr: %-10s\n", my_dictionary[i].first, my_dictionary[i].second);
    }
    printf("\n");
}

const char *search_into_dictionary(vector<pair<const char *, const char *>> my_dictionary, const char *eng_word)
{
    for (int i = 0; i < my_dictionary.size(); i++)
    {
        if (!strcmp(eng_word, my_dictionary[i].first))
            return my_dictionary[i].second;
    }
    return "NULL";
}

void show_message(int out, string message)
{
    if (out < 0)
    {
        cout << message << endl;
        exit(0);
    }
}

void print_buffer(string action, char buffer[], size_t bytes)
{
    printf("%s: %s (%ld bytes)\n", action.c_str(), buffer, bytes);
}
