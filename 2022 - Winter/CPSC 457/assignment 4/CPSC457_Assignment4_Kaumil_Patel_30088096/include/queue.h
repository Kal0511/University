#ifndef ASSIGNMENT_4_QUEUE_H
#define ASSIGNMENT_4_QUEUE_H

#include <mutex>
#include <condition_variable>

using namespace std;

// generic dynamic data structure to store students in line
template<class T>
class queue {

    struct node {
        T *data;
        node *prev;
        node *next;

        node(T *d, node *p = nullptr, node *n = nullptr) : data(d), prev(p), next(n) {}
    } typedef node;

private:
    node *head;
    node *tail;
    size_t size;
    mutex queueMutex;
    condition_variable queueCondition;
    bool open;
public:
    queue() : head(nullptr), tail(nullptr), size(0), open(true) {}

    ~queue(){
        deleteQueue(head);
    }

    void deleteQueue(node* curr){
        if(curr == nullptr){
            return;
        }
        deleteQueue(curr->next);
        delete curr;
    }

    // used to add student to the queue
    void push(T *add) {
        lock_guard<mutex> lock(queueMutex);
        if (head == nullptr) {
            head = new node(add);
            tail = head;
        } else {
            tail->next = new node(add, tail);
            tail = tail->next;
        }
        size++;

        queueCondition.notify_one();
    }

    // used to close the queue
    void closeQueue() {
        lock_guard<mutex> lock(queueMutex);
        open = false;
    }

    void awaitPush() {
        unique_lock<mutex> lock(queueMutex);
        if (open) {
            while (size == 0) {
                queueCondition.wait(lock);
                if (!open) {
                    break;
                }
            }
        }
    }

    // used to remove student to the queue
    T *pop() {
        unique_lock<mutex> lock(queueMutex);
        if (open) {
            while (size == 0) {
                queueCondition.wait(lock);
                if (!open) {
                    if (size == 0) {
                        return nullptr;
                    }else{
                        break;
                    }
                }
            }
        }else{
            if (size == 0) {
                return nullptr;
            }
        }
        T *first = head->data;
        node *next = head->next;
        delete head;
        head = next;
        size--;
        return first;
    }

    bool isEmpty() {
        lock_guard<mutex> lock(queueMutex);
        return size == 0;
    }

    bool isClosed() {
        lock_guard<mutex> lock(queueMutex);
        return !open;
    }

    void refresh() {
        lock_guard<mutex> lock(queueMutex);
        queueCondition.notify_all();
    }
};

#endif
