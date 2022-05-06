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

// currency struct
struct Currency
{
    char src_currency[4];
    char dst_currency[4];
    float src_value = 1;
    float rate;
    float dst_value = rate;
};

// functions
void fill_table(vector<Currency> &my_table);
void print_table(vector<Currency> my_table);
float convert(vector<Currency> my_table, Currency user_inp);
void show_message(int out, string message);
void print_buffer(string action, char buffer[], size_t bytes);
Currency create_currency(char const user_input[]);

/*
Main function 
 */
int main(int argc, char const *argv[])
{
    vector<Currency> currency_exchange_table;
    fill_table(currency_exchange_table);

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
    print_table(currency_exchange_table);

    for (;;)
    {
        // receive from main server
        memset(buffer, 0, sizeof(buffer));
        size_t recv_size = recvfrom(micro_socket, buffer, sizeof(buffer), 0, micro_info->ai_addr, &micro_info->ai_addrlen);
        print_buffer("Received", buffer, recv_size);

        // get info from user input
        Currency user_input = create_currency(buffer);
        memset(buffer, 0, sizeof(buffer));

        // sprintf(buffer, "%f", convert(currency_exchange_table, user_input));
        // snprintf(buffer, sizeof buffer, "%f", myFloat);
        int r = snprintf(buffer, sizeof(buffer), "%f", convert(currency_exchange_table, user_input));
        // printf("%s : %ld", buffer, r);

        size_t send_size = sendto(micro_socket, buffer, sizeof(buffer), 0, micro_info->ai_addr, micro_info->ai_addrlen);
        show_message(send_size, "send() failed");
        print_buffer("Sent", buffer, send_size);
    }
}

// Fill Dictionary - Call by reference
void fill_table(vector<Currency> &my_table)
{
    Currency CAD_USD = {.src_currency = "CAN", .dst_currency = "USD", .rate = 0.79};
    Currency CAD_EUR = {.src_currency = "CAN", .dst_currency = "EUR", .rate = 0.69};
    Currency CAD_GBP = {.src_currency = "CAN", .dst_currency = "GBP", .rate = 0.58};
    Currency CAD_BTC = {.src_currency = "CAN", .dst_currency = "BTC", .rate = 0.000016};
    Currency CAD_GPY = {.src_currency = "CAN", .dst_currency = "GPY", .rate = 88.62};

    my_table.push_back(CAD_USD);
    my_table.push_back(CAD_EUR);
    my_table.push_back(CAD_GBP);
    my_table.push_back(CAD_BTC);
    my_table.push_back(CAD_GPY);
}

void print_table(vector<Currency> my_table)
{
    printf("\n\tCurrency Exchange Table\n");
    for (int i = 0; i < my_table.size(); i++)
    {
        printf("%s to %s = %f\n", my_table[i].src_currency, my_table[i].dst_currency, my_table[i].rate);
    }
    printf("\n");
}

float convert(vector<Currency> my_table, Currency user_inp)
{
    for (int i = 0; i < my_table.size(); i++)
    {
        if (!strcmp(user_inp.dst_currency, my_table[i].dst_currency))
        {
            user_inp.dst_value = user_inp.src_value * my_table[i].rate;
            return user_inp.dst_value;
        }
    }
    return -1;
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

Currency create_currency(char const user_input[])
{
    string s(user_input);
    string delimiter = " ";

    Currency user_currency = {.src_currency = "CAD", .dst_currency = "USD", .src_value = 20, .rate = 3.2, .dst_value = 0};
    size_t pos = s.find(delimiter);
    strcpy(user_currency.src_currency, s.substr(0, pos).c_str());
    s.erase(0, pos + delimiter.length()); // remove

    pos = s.find(delimiter);
    strcpy(user_currency.dst_currency, s.substr(0, pos).c_str());
    s.erase(0, pos + delimiter.length()); // remove

    user_currency.src_value = stof(s);

    return user_currency;
}
