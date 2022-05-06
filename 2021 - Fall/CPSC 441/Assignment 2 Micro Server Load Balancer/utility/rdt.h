// CPSC 441 - Assignment 2 - Utility Functions
// made by Kaumil Patel 30088096

#include "string"

using namespace std;


int sockSize = sizeof(struct sockaddr_in);
char ack[] = "ACK";

// Fletcher16
uint16_t checksum(const char data[]) {
    uint16_t sum1 = 0;
    uint16_t sum2 = 0;
    for (int index = 0; data[index] != '\0'; ++index) {
        sum1 = (sum1 + data[index]) % 255;
        sum2 = (sum2 + sum1) % 255;
    }
    return (sum2 << 8) | sum1;
}

// makes udp packet based on data and sequence number and stores result back in data
void makePacket(char data[], int seqNum) {
    char temp[512];
    sprintf(temp, "%d %s", seqNum, data);
    uint16_t sum = checksum(temp);
    sprintf(data, "%04X %s", sum, temp);
}

// makes udp packet based on data and sequence number and stores result in dest
void makePacket(char dest[], char data[], int seqNum) {
    char temp[512];
    sprintf(temp, "%d %s", seqNum, data);
    uint16_t sum = checksum(temp);
    sprintf(dest, "%04X %s", sum, temp);
}

// get dequence number from data packet
int getSeqNum(char data[]) {
    return data[5] - '0';
}

// checks if packet contains an ack and right sequence number
bool isAck(char data[], int seqNum) {
    if (getSeqNum(data) == seqNum) {
        if (strncmp(&data[7], ack, 3) == 0) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

// checks id packet is corrupted based in the sequence number calculated and the one it contains
bool isCorrupted(char data[]) {
    int sum = stoi(&data[0], nullptr, 16);
    if (sum != checksum(&data[5])) {
        return true;
    } else {
        return false;
    }
}

// sends data packet to the addr provided using udp socket
void sendRDT(int &senderSockfd, struct sockaddr_in &destAddr, bool &seqNum, char outgoingBuffer[], char incomingBuffer[], int &bufferSize, int &errorCode) {
    makePacket(outgoingBuffer, seqNum);
    while (true) {
        if ((errorCode = sendto(senderSockfd, outgoingBuffer, strlen(outgoingBuffer), 0, (struct sockaddr *) &destAddr, sockSize)) < 0) {
            return;
        }
        cout << "UDP Sent: " << outgoingBuffer << endl;
        if ((errorCode = recvfrom(senderSockfd, incomingBuffer, bufferSize, 0, (struct sockaddr *) &destAddr, &sockSize)) < 0) {
            return;
        }
        incomingBuffer[errorCode] = '\0';
        cout << "UDP Received: " << incomingBuffer << endl;
        if (!isCorrupted(incomingBuffer) && isAck(incomingBuffer, seqNum)) {
            seqNum = !seqNum;
            return;
        }
    }
}

// receives data packet to the addr provided using udp socket
void recvRDT(int &senderSockfd, struct sockaddr_in &destAddr, char outgoingBuffer[], char incomingBuffer[], int &bufferSize, int &errorCode, char *&body) {
    while (true) {
        if ((errorCode = recvfrom(senderSockfd, incomingBuffer, bufferSize, 0, (struct sockaddr *) &destAddr, &sockSize)) < 0) {
            return;
        }
        bool seqNum = getSeqNum(incomingBuffer);
        incomingBuffer[errorCode] = '\0';
        body = incomingBuffer + 7;
        cout << "UDP Received: " << incomingBuffer << endl;
        if (isCorrupted(incomingBuffer)) {
            makePacket(outgoingBuffer, ack, !seqNum);
            cout << "UDP Sent: " << outgoingBuffer << endl;
            if ((errorCode = sendto(senderSockfd, outgoingBuffer, strlen(outgoingBuffer), 0, (struct sockaddr *) &destAddr, sockSize)) < 0) {
                return;
            }
        } else {
            makePacket(outgoingBuffer, ack, seqNum);
            cout << "UDP Sent: " << outgoingBuffer << endl;
            if ((errorCode = sendto(senderSockfd, outgoingBuffer, strlen(outgoingBuffer), 0, (struct sockaddr *) &destAddr, sockSize)) < 0) {
                return;
            }
            return;
        }
    }
}