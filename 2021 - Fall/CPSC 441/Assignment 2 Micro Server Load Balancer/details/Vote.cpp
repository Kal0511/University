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
#include <string>
#include <iomanip>
#define MAIN_SERVER_IP "136.159.5.25"
#define MAIN_SERVER_PORT "8888"
#define BUFSIZE 256
#define encode_key "5"

// General variables

using namespace std;

// enum to keep command keywords

// enum commands
// {
//     KEY,
//     LIST,
//     VOTE,
//     RESULT
// };

// candidate struct data type
struct Candiate
{
    const char candiate_name[15];
    const char candiate_family[15];
    int id;
    int votes;
};

/* 
Functions
 */
size_t reply_back(int peer_socket, addrinfo *micro_info, const char message[], sockaddr_storage peer_addr);
void fill_table(vector<Candiate> &my_table);
void print_table(vector<Candiate> my_table);
string return_candidates_info(vector<Candiate> my_table);
string return_candidates_list(vector<Candiate> my_table);
void show_message(int out, string message);
int vote(int peer_socket, addrinfo *micro_info, vector<string> server_message, vector<Candiate> &candidates_table, sockaddr_storage peer_addr);
size_t reply_back(int peer_socket, addrinfo *micro_info, const char message[], sockaddr_storage peer_addr);
vector<string> parse_message(const char buffer[]);
void print_buffer(string action, char buffer[], size_t bytes);

int main(int argc, char const *argv[])
{
    // initiate a sample candiates's voting table
    vector<Candiate> voting_table;
    fill_table(voting_table);

    // a vector to keep voters' IP addresses
    vector<string> voters;

    char buffer[BUFSIZE];

    // Listen to a UDP socket
    addrinfo hints;
    addrinfo *micro_info;
    struct sockaddr_storage peer_addr;
    socklen_t peer_addr_len;
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
    print_table(voting_table);

    peer_addr_len = sizeof(sockaddr_storage);
    for (;;)
    {
        // a vector to parse and keep users' messages
        vector<string> vec_server_message;

        // receive from main server
        memset(buffer, 0, sizeof(buffer));
        size_t recv_size = recvfrom(micro_socket, buffer, sizeof(buffer), 0, (struct sockaddr *)&peer_addr, &peer_addr_len);

        // size_t recv_size = recvfrom(micro_socket, buffer, sizeof(buffer), 0, micro_info->ai_addr, micro_info->ai_addrlen);
        print_buffer("\nReceived", buffer, recv_size);
        // Parse the server message
        vec_server_message = parse_message(buffer);

        if (!strcmp(vec_server_message[0].c_str(), "KEY"))
        {
            int temp = 1;
            string str_ip = vec_server_message[1];

            for (int j = 0; j < voters.size(); j++)
            {
                if (!strcmp(str_ip.c_str(), voters[j].c_str()))
                {
                    reply_back(micro_socket, micro_info, "NO", peer_addr);
                    temp = 0;
                    break;
                }
            }
            if (temp)
                reply_back(micro_socket, micro_info, encode_key, peer_addr);
        }
        else if (!strcmp(vec_server_message[0].c_str(), "LIST"))
            reply_back(micro_socket, micro_info, return_candidates_list(voting_table).c_str(), peer_addr);
        else if (!strcmp(vec_server_message[1].c_str(), "VOTE"))
        {

            int temp = vote(micro_socket, micro_info, vec_server_message, voting_table, peer_addr);
            if (temp != -1)
                voters.push_back(vec_server_message[2]);
        }
        else if (!strcmp(vec_server_message[0].c_str(), "RESULT"))
        {
            int temp = 0;
            string str_ip = vec_server_message[1];

            for (int j = 0; j < voters.size(); j++)
            {
                if (!strcmp(str_ip.c_str(), voters[j].c_str()))
                {
                    cout << "SINA  " << str_ip << endl
                         << voters[j] << endl;
                    reply_back(micro_socket, micro_info, return_candidates_info(voting_table).c_str(), peer_addr);
                    temp = 1;
                }
            }
            if (temp == 0)
            {
                reply_back(micro_socket, micro_info, "NO", peer_addr);
            }
        }
        else
        {
            printf("%c is not defined!\n", vec_server_message[0]);
        }
    }
}
// add some candidate to the voting table
void fill_table(vector<Candiate> &my_table)
{
    Candiate Berners_Lee = {"Tim", "Berners-Lee", 1111, 33};
    Candiate Kahn = {"Robert", "Kahn", 2222, 88};
    Candiate Kurose = {"Jim", "Kurose", 3333, 14};
    Candiate Stroustrup = {"Bjarne", "Stroustrup", 4444, 44};
    Candiate Williamson = {"Carey", "Williamson", 5555, 99};

    my_table.push_back(Berners_Lee);
    my_table.push_back(Kahn);
    my_table.push_back(Kurose);
    my_table.push_back(Stroustrup);
    my_table.push_back(Williamson);
}

// show voting table on the output terminal
void print_table(vector<Candiate> my_table)
{
    printf("\n\tCandiate Table\n");
    printf("%-8s  %-12s  %-5s  %-3s\n\n", "Name", "Family", "ID", "Votes");
    for (int i = 0; i < my_table.size(); i++)
    {
        printf("%-8s  %-12s  %-5d  %-3d\n", my_table[i].candiate_name, my_table[i].candiate_family, my_table[i].id, my_table[i].votes);
    }
    printf("\n");
}

string return_candidates_info(vector<Candiate> my_table)
{
    string candidates = "\n\tCandiate Table\n";
    for (int i = 0; i < my_table.size(); i++)
    {
        candidates += (my_table[i].candiate_name + string(" ") + my_table[i].candiate_family + string(" ") + to_string(my_table[i].id) + string(" ") + to_string(my_table[i].votes) + string("\n"));
    }
    return (candidates);
}

string return_candidates_list(vector<Candiate> my_table)
{
    string candidates = "\n\tCandiate Table\n";
    for (int i = 0; i < my_table.size(); i++)
    {
        candidates += (my_table[i].candiate_name + string(" ") + my_table[i].candiate_family + string(" ") + to_string(my_table[i].id) + string("\n"));
    }
    return (candidates);
}

void show_message(int out, string message)
{
    if (out < 0)
    {
        cout << message << endl;
        exit(0);
    }
}
// return_candidates_info
//     printf("%s: %s (%ld bytes)\n", action.c_str(), buffer, bytes);
// }

int vote(int peer_socket, addrinfo *micro_info, vector<string> server_message, vector<Candiate> &candidates_table, sockaddr_storage peer_addr)
{
    // get the encoded vote
    int user_vote = stoi(server_message[0]);

    // decode the vote
    user_vote /= atoi(encode_key);

    for (int i = 0; i < candidates_table.size(); i++)
    {
        if (candidates_table[i].id == user_vote)
        {
            candidates_table[i].votes += 1;

            reply_back(peer_socket, micro_info, "Thank You for Voting!", peer_addr);
            return (candidates_table[i].votes);
        }
    }
    reply_back(peer_socket, micro_info, "Voting ID is wrong!", peer_addr);
    return (-1); // it there is not any candidate with this id, Reply -1
}
size_t reply_back(int peer_socket, addrinfo *micro_info, const char message[], sockaddr_storage peer_addr)
{
    socklen_t peer_addr_len;
    peer_addr_len = sizeof(struct sockaddr_storage);
    char buffer[BUFSIZE];
    memset(buffer, 0, sizeof(buffer));
    strcpy(buffer, message);
    size_t send_size = sendto(peer_socket, buffer, sizeof(buffer), 0, (sockaddr *)&peer_addr, peer_addr_len);
    printf("Sent: %s: (%ld bytes)\n", buffer, send_size);
    return send_size;
}

vector<string> parse_message(const char buffer[])
{
    string recv_txt(buffer);
    string delimiter = " ";
    vector<string> vec_words; // to keep words in a message
    int pos;                  // to find space in a message
    while ((pos = recv_txt.find(delimiter)) != -1)
    {
        // cout << "PARSE " << recv_txt.substr(0, pos) << endl;
        vec_words.push_back(recv_txt.substr(0, pos));
        recv_txt.erase(0, pos + delimiter.length());
    }
    vec_words.push_back(recv_txt);
    return vec_words;
}

// simple function to print buffer content and size
void print_buffer(string action, char buffer[], size_t bytes)
{
    printf("%s: %s (%ld bytes)\n", action.c_str(), buffer, bytes);
}