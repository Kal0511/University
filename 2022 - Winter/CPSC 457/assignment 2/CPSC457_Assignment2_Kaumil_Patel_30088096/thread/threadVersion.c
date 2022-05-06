#include <stdio.h>
#include <sys/unistd.h>
#include <semaphore.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define MAX_TRAY_SIZE 10
#define MAX_BUFFER_SIZE 512

// struct used to store tray information
typedef struct queue {
    int tray[MAX_TRAY_SIZE];
    int length;
    sem_t lock;
    sem_t availableChef;
    sem_t availableConsumer;
} queue;

int fd[2];
queue veganTray;
queue nonVeganTray;

// initialize queue in shared memory
queue queueInit() {
    queue tray;
    tray.length = 0;
    sem_init(&tray.lock, 0, 1);
    sem_init(&tray.availableChef, 0, MAX_TRAY_SIZE);
    sem_init(&tray.availableConsumer, 0, 0);
    return tray;
}

// takes in a tray and adds a dish
int produce(queue *buffer) {
    sem_wait(&buffer->availableChef);
    int randType = rand() % 2 + 1;
    sem_wait(&buffer->lock);
    buffer->tray[buffer->length] = randType;
    buffer->length++;
    sem_post(&buffer->availableConsumer);
    sem_post(&buffer->lock);
    return randType;
}

// takes in a tray and removes a dish
int consume(queue *buffer) {
    sem_wait(&buffer->availableConsumer);
    sem_wait(&buffer->lock);
    buffer->length--;
    int type = buffer->tray[buffer->length];
    sem_post(&buffer->availableChef);
    sem_post(&buffer->lock);
    return type;
}

// vegan producer process
void *veganChef(void *arg) {
    while (1) {
        int type = produce(&veganTray);
        if (type == 1) {
            char output[] = "Portecelli Produced Pistachio Pesto Pasta\n";
            write(fd[1], output, strlen(output));
        } else {
            char output[] = "Portecelli Produced Avocado Fruit Salad\n";
            write(fd[1], output, strlen(output));
        }
        int sleepLen = rand() % 5 + 1;
        sleep(sleepLen);
    }
}

// non vegan producer process
void *nonVeganChef(void *arg) {
    while (1) {
        int type = produce(&nonVeganTray);
        if (type == 1) {
            char output[] = "Donatello Produced Fettuccine Chicken Alfredo\n";
            write(fd[1], output, strlen(output));
        } else {
            char output[] = "Donatello Produced Garlic Sirloin Steak\n";
            write(fd[1], output, strlen(output));
        }
        int sleepLen = rand() % 5 + 1;
        sleep(sleepLen);
    }
}

// non vegan consumer process
void *nonVeganConsumer(void *arg) {
    while (1) {
        int type = consume(&nonVeganTray);
        if (type == 1) {
            char output[] = "Non Vegan Consumer Ate Fettuccine Chicken Alfredo\n";
            write(fd[1], output, strlen(output));
        } else {
            char output[] = "Non Vegan Consumer Ate Garlic Sirloin Steak\n";
            write(fd[1], output, strlen(output));
        }
        int sleepLen = rand() % 6 + 10;
        sleep(sleepLen);
    }
}

// vegan consumer process
void *veganConsumer(void *arg) {
    while (1) {
        int type = consume(&veganTray);
        if (type == 1) {
            char output[] = "Vegan Consumer Ate Pistachio Pesto Pasta\n";
            write(fd[1], output, strlen(output));
        } else {
            char output[] = "Vegan Consumer Ate Avocado Fruit Salad\n";
            write(fd[1], output, strlen(output));
        }
        int sleepLen = rand() % 6 + 10;
        sleep(sleepLen);
    }
}

// hybrid consumer process
void *bothConsumer(void *arg) {
    while (1) {
        int veganType = consume(&veganTray);
        int nonVeganType = consume(&nonVeganTray);
        if (veganType == 1) {
            if (nonVeganType == 1) {
                char output[] = "Hybrid Consumer Ate Pistachio Pesto Pasta And Fettuccine Chicken Alfredo\n";
                write(fd[1], output, strlen(output));
            } else {
                char output[] = "Hybrid Consumer Ate Pistachio Pesto Pasta And Garlic Sirloin Steak\n";
                write(fd[1], output, strlen(output));
            }
        } else {
            if (nonVeganType == 1) {
                char output[] = "Hybrid Consumer Ate Avocado Fruit Salad And Fettuccine Chicken Alfredo\n";
                write(fd[1], output, strlen(output));
            } else {
                char output[] = "Hybrid Consumer Ate Avocado Fruit Salad And Garlic Sirloin Steak\n";
                write(fd[1], output, strlen(output));
            }
        }
        int sleepLen = rand() % 6 + 10;
        sleep(sleepLen);
    }
}

// log production
void *historyLog(void *arg) {
    char readBuffer[MAX_BUFFER_SIZE];
    while (1) {
        sem_wait(&veganTray.lock);
        sem_wait(&nonVeganTray.lock);
        printf("AWAKE\n");
        printf("Vegan Tray %d/%d | ", veganTray.length, MAX_TRAY_SIZE);
        printf("Non Vegan Tray %d/%d\n\n", nonVeganTray.length, MAX_TRAY_SIZE);
        while (1) {
            int n = read(fd[0], readBuffer, MAX_BUFFER_SIZE);
            readBuffer[n] = 0;
            printf("%.*s", n, readBuffer);
            if (n < MAX_BUFFER_SIZE) {
                break;
            }
        }
        printf("Time To Sleep\n\n");
        sem_post(&veganTray.lock);
        sem_post(&nonVeganTray.lock);
        sleep(10);
    }
}

int main() {
    // initialize trays
    veganTray = queueInit();
    nonVeganTray = queueInit();

    pipe(fd);

    // start producer
    pthread_t producer1, producer2;
    pthread_create(&producer1, NULL, veganChef, NULL);
    pthread_create(&producer2, NULL, nonVeganChef, NULL);

    // start log
    pthread_t log;
    pthread_create(&log, NULL, historyLog, NULL);

    // start consumer
    pthread_t consumer1, consumer2, consumer3;
    pthread_create(&consumer1, NULL, veganConsumer, NULL);
    pthread_create(&consumer2, NULL, nonVeganConsumer, NULL);
    pthread_create(&consumer3, NULL, bothConsumer, NULL);

    pthread_join(producer1, NULL);
    pthread_join(producer2, NULL);
    pthread_join(consumer1, NULL);
    pthread_join(consumer2, NULL);
    pthread_join(consumer3, NULL);
    pthread_join(log, NULL);
}
