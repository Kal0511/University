/* TCP-based server example of socket programming.    */
/* CPSC 441 test server with simple commands.         */
/*                                                    */
/* Usage: cc -o testserver testserver.c               */
/*        ./testserver                                */
/*                                                    */
/* Written by Carey Williamson        August 25, 2021 */

/* Include files for C socket programming and stuff */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <signal.h>
#include <string.h>
#include <arpa/inet.h>

/* Global manifest constants */
#define MAX_MESSAGE_LENGTH 100
#define SERVERPORTNUM 44101

/* Optional verbose debugging output */
#define DEBUG 1

/* Global variable */
int childsockfd;

/* This is a signal handler to do graceful exit if needed */
void catcher( int sig )
{
    close(childsockfd);
    exit(0);
}

/* Main program for server */
int main()
{
    struct sockaddr_in server, client;
    static struct sigaction act;
    char messagein[MAX_MESSAGE_LENGTH];
    char messageout[MAX_MESSAGE_LENGTH];
    int parentsockfd, c, pid, conns;

    /* Set up a signal handler to catch any unusual termination conditions. */
    act.sa_handler = catcher;
    sigfillset(&(act.sa_mask));
    sigaction(SIGPIPE, &act, NULL);

    /* Initialize server sockaddr structure */
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVERPORTNUM);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    /* set up the transport-level end point to use TCP */
    if( (parentsockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1 )
    {
        fprintf(stderr, "testserver: socket() call failed!\n");
        exit(1);
    }

    /* bind a specific address and port to the end point */
    if( bind(parentsockfd, (struct sockaddr *)&server, sizeof(struct sockaddr_in) ) == -1 )
    {
        fprintf(stderr, "testserver: bind() call failed!\n");
        exit(1);
    }

    /* start listening for incoming connections from clients */
    if( listen(parentsockfd, 5) == -1 )
    {
        fprintf(stderr, "testserver: listen() call failed!\n");
        exit(1);
    }

    /* initialize message strings just to be safe (null-terminated) */
    bzero(messagein, MAX_MESSAGE_LENGTH);
    bzero(messageout, MAX_MESSAGE_LENGTH);

    fprintf(stderr, "Welcome! I am the CPSC 441 test server for L01!!\n");
    fprintf(stderr, "Server listening on TCP port %d...\n\n", SERVERPORTNUM);

    conns = 0;
    c = sizeof(struct sockaddr_in);

    /* Main loop: server loops forever listening for requests */
    for( ; ; )
    {
        /* accept a connection */
        if( (childsockfd = accept(parentsockfd, (struct sockaddr *)&client, (socklen_t *) &c)) == -1 )
        {
            fprintf(stderr, "testserver: accept() call failed!\n");
            exit(1);
        }

        /* increment server's counter variable */
        conns++;

        /* try to create a child process to deal with this new client */
        pid = fork();

        /* use process id (pid) returned by fork to decide what to do next */
        if( pid < 0 )
        {
            fprintf(stderr, "testserver: fork() call failed!\n");
            exit(1);
        }
        else if( pid == 0 )
        {
            /* this is the child process doing this part */

            /* don't need the parent listener socket that was inherited */
            close(parentsockfd);

            /* obtain the message from this client */
            int bytes;
            int done = 0;
            while( !done )
            {
                bytes = recv(childsockfd, messagein, MAX_MESSAGE_LENGTH, 0);
#ifdef DEBUG
                /* print out the received message */
                printf("Child process received %d bytes with command: <%s>\n",
                       bytes, messagein);
#endif

                /* create the outgoing message based on command */
                if( strncmp(messagein, "HELLO", 5) == 0 )
                    sprintf(messageout, "Hello to you as well!\n");
                else if( strncmp(messagein, "COUNT", 5) == 0 )
                    sprintf(messageout, "You are client #%d since I was started.\n", conns);
                else if( strncmp(messagein, "HELP", 4) == 0 )
                    sprintf(messageout, "Supported commands:\n  HELLO\n  INFO\n  COUNT\n  HELP\n  BYE\n", conns);
                else if( strncmp(messagein, "INFO", 4) == 0 )
                    sprintf(messageout,
                            "Your transport-level endpoint is IP %s port %d\n",
                            inet_ntoa(client.sin_addr), ntohs(client.sin_port));
                else if( strncmp(messagein, "BYE", 3) == 0 )
                {
                    sprintf(messageout, "Goodbye! See you next time.\n");
                    done = 1;
                }
                else sprintf(messageout, "Unrecognized command. Sorry!\n");

#ifdef DEBUG
                printf("Child about to send message: %s\n", messageout);
#endif

                /* send the result message back to the client */
                if( send(childsockfd, messageout, strlen(messageout), 0) < 0)
                {
                    fprintf(stderr, "Send failed on connection %d\n", conns);
                }

                /* clear out message strings again to be safe */
                bzero(messagein, MAX_MESSAGE_LENGTH);
                bzero(messageout, MAX_MESSAGE_LENGTH);
            }

            /* when client is no longer sending information to us, */
            /* the socket can be closed and the child process terminated */
            fprintf(stderr, "Shutting down child process for conn %d\n", conns);
            close(childsockfd);
            exit(0);
        } /* end of then part for child */
        else
        {
            /* the parent process is the one doing this part */
            fprintf(stderr, "Created child process %d to handle that client\n", pid);
            fprintf(stderr, "Parent going back to job of listening...\n\n");

            /* parent doesn't need the childsockfd */
            close(childsockfd);
        }
    }
}