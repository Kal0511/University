#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

// Max arguments from a command
#define MaxArgs 32
// Max input length
#define MaxLength 512

// recursive execution function for commands
void executeRecursive(char *input);

// process rew command into argument list
void processCommand(char command[], char *argList[]) {
    for (int i = 0; i < MaxArgs; i++) {
        argList[i] = strsep(&command, " ");
        if (argList[i] == NULL) {
            break;
        }
        if (strlen(argList[i]) == 0) {
            i--;
        }
    }
}

// find symbol in given command
char *findSymbol(char *command, char symbol) {
    for (char *i = command; *i != '\0'; i++) {
        if (*i == symbol) {
            *i = '\0';
            i++;
            return i;
        }
    }
    return NULL;
}

// get filename from a command
char *getFilename(char *command) {
    for (char *i = command; *i != '\0'; i++) {
        if (*i == ' ') {
            continue;
        } else {
            return strsep(&i, " ");
        }
    }
    return NULL;
}

// basic command execution function
void executeCommand(char *command) {
    char *argumentList[MaxArgs];
    char *pipePos;

    // if output function then create file
    if ((pipePos = findSymbol(command, '>')) != NULL) {
        int writefd = creat(getFilename(pipePos), 0644);
        if (writefd == -1) {
            perror("Can't create file");
            exit(1);
        }
        dup2(writefd, STDOUT_FILENO);
        close(writefd);

        // if input function then read file
    } else if ((pipePos = findSymbol(command, '<')) != NULL) {
        int readfd = open(getFilename(pipePos), O_RDONLY, 0);
        if (readfd == -1) {
            perror("File doesn't exist");
            exit(1);
        }
        dup2(readfd, STDIN_FILENO);
        close(readfd);
    }
    processCommand(command, argumentList);

    // empty command error checking
    if(*argumentList == NULL){
        perror("Empty command");
        exit(1);
    }
    if (execvp(argumentList[0], argumentList) == -1) {
        perror("Command error");
        exit(1);
    }
}

// check if command is a background function
int isBackgroundCommand(char *command) {
    for (char *i = command; *i != '\0'; i++) {
        if (*i == '&') {
            *i = '\0';
            return 1;
        }
    }
    return 0;
}

// recursive pipe command function
void pipeCommand(char *commandLeft, char *commandRight) {
    int pipefd[2];
    if (pipe(pipefd) < 0) {
        perror("Pipe error");
        exit(1);
    }
    int pid1 = fork();
    if (pid1 < 0) {
        perror("Fork error");
        exit(1);
    } else if (pid1 == 0) {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        executeRecursive(commandLeft);
    } else {
        int pid2 = fork();
        if (pid2 < 0) {
            perror("Fork error");
            exit(1);
        } else if (pid2 == 0) {
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            executeRecursive(commandRight);
        } else {
            close(pipefd[0]);
            close(pipefd[1]);
            waitpid(pid1, NULL, 0);
            waitpid(pid2, NULL, 0);
            exit(0);
        }
    }
}

// recursive different pipe command function
void multiPipeCommand(char *commandLeft, char *commandRight) {
    char *argumentList[MaxArgs];
    char *argumentListPiped[MaxArgs];
    processCommand(commandLeft, argumentList);
    if(*argumentList == NULL){
        perror("Empty command");
        exit(1);
    }
    int pipefd[2];
    if (pipe(pipefd) < 0) {
        perror("Pipe error");
        exit(1);
    }
    int pid1 = fork();
    if (pid1 < 0) {
        perror("Fork error");
        exit(1);
    } else if (pid1 == 0) {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        for (char **command = argumentList; *command != NULL; command++) {
            int pid = fork();
            if (pid == -1) {
                perror("Fork error");
                exit(1);
            } else if (pid == 0) {
                if (execlp(*argumentList, *argumentList, NULL) == -1) {
                    perror("Command error");
                    exit(1);
                }
            } else {
                waitpid(pid, NULL, 0);
            }
        }
        exit(0);
    } else {
        waitpid(pid1, NULL, 0);
        int pid2 = fork();
        if (pid2 < 0) {
            perror("Fork error");
            exit(1);
        } else if (pid2 == 0) {
            close(pipefd[1]);
            char *pos;
            if ((pos = findSymbol(commandRight, '$')) != NULL) {
                dup2(pipefd[0], STDIN_FILENO);
                close(pipefd[0]);
                multiPipeCommand(commandRight, pos);
                exit(0);
            }
            processCommand(commandRight, argumentListPiped);
            if(*argumentListPiped == NULL){
                perror("Empty command");
                exit(1);
            }
            int count = 0;
            for (char **command = argumentListPiped; *command != NULL; command++) {
                count++;
            }
            int pipefdcopy[2 * count];
            char data[MaxLength * 2];
            long len;
            for (int i = 0; i < count; i++) {
                pipe(&pipefdcopy[i * 2]);
            }
            while ((len = read(pipefd[0], data, MaxLength * 2)) > 0) {
                for (int i = 0; i < count; i++) {
                    write(pipefdcopy[i * 2 + 1], data, len);
                }
            }
            for (int i = 0; i < count; i++) {
                close(pipefdcopy[i * 2 + 1]);
            }
            int i = 0;
            for (char **command = argumentListPiped; *command != NULL; command++) {
                int pid = fork();
                if (pid == -1) {
                    perror("Fork error");
                    exit(1);
                } else if (pid == 0) {
                    dup2(pipefdcopy[i * 2], STDIN_FILENO);
                    close(pipefdcopy[i * 2]);
                    if (execlp(*argumentListPiped, *argumentListPiped, NULL) == -1) {
                        perror("Command error");
                        exit(1);
                    }
                } else {
                    waitpid(pid, NULL, 0);
                    close(pipefdcopy[i * 2]);
                }
                i++;
            }
            exit(0);
        } else {
            close(pipefd[0]);
            close(pipefd[1]);
            waitpid(pid2, NULL, 0);
            exit(0);
        }
    }
}

// recursive execution function for commands
void executeRecursive(char *input) {
    char *pos;
    if ((pos = findSymbol(input, '$')) != NULL) {
        multiPipeCommand(input, pos);
    } else if ((pos = findSymbol(input, '|')) != NULL) {
        pipeCommand(input, pos);
    } else {
        executeCommand(input);
    }
}

int main() {
    printf("\nWelcome to Open Shell (enter 'exit' to close shell)\n\n");
    char cwd[MaxLength];
    if (getcwd(cwd, MaxLength) == NULL) {
        perror("Can't get working directory error");
        exit(1);
    }
    char input[MaxLength];
    printf("%s: ", cwd);

    // main loop
    while (1) {
        fgets(input, MaxLength, stdin);
        if (input[strlen(input) - 1] == '\n') {
            input[strlen(input) - 1] = '\0';
        }
        if (strncmp(input, "exit", 4) == 0) {
            printf("\nOpen Shell Closed\n");
            exit(0);
        }
        int isNotBackgroundCommand;
        if (isBackgroundCommand(input)) {
            isNotBackgroundCommand = 0;
        } else {
            isNotBackgroundCommand = 1;
        }
        int pid = fork();
        if (pid == -1) {
            perror("Fork error");
        } else if (pid == 0) {
            executeRecursive(input);
        } else {
            if (isNotBackgroundCommand) {
                waitpid(pid, NULL, 0);
                printf("%s: ", cwd);
            }
        }
    }
}