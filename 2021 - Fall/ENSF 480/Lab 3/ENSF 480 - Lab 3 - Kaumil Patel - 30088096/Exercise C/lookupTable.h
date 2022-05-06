/*
* File Name: lookupTable.h
* Assignment: Lab 3 Exercise C
* Lab Section: B01
* Completed by: Kaumil Patel
* Submission Date: Oct 14, 2021
*/

#ifndef LookupTable_H
#define LookupTable_H

#include <iostream>

using namespace std;

// class LookupTable<T, K>: GENERAL CONCEPTS
//
//    key/datum pairs are ordered.  The first pair is the pair with
//    the lowest key, the second pair is the pair with the second
//    lowest key, and so on.  This implies that you must be able to
//    compare two keys with the < operator.
//
//    Each LookupTable<T, K> has an embedded iterator class that allows users
//    of the class to traverse trhough the list and  have acess to each
//    node. 

#include "customer.h"

//    In this version of the LookupTable<T, K> a new struct type called Pair
//    is introduced which represents a key/data pair.


template<class T, class K>
struct Pair {
    Pair(T keyA, K datumA) : key(keyA), datum(datumA) {
    }
    T key;
    K datum;
};

template<typename T, typename K> class LookupTable;
template<typename T, typename K>
ostream& operator<< (ostream &, const LookupTable<T,K>& lt);

template<class T, class K>
class LT_Node {
    friend class LookupTable<T, K>;

private:
    Pair<T, K> pairM;
    LT_Node *nextM;

    // This ctor should be convenient in insert and copy operations.
    LT_Node(const Pair<T, K> &pairA, LT_Node *nextA);
};

template<class T, class K>
class LookupTable {
public:
    // Nested class
    class Iterator {
        friend class LookupTable;

        LookupTable *LT;
//    LT_Node* cursor;
    public:
        Iterator() : LT(0) {}

        Iterator(LookupTable &x) : LT(&x) {}

        const K &operator*();

        const K &operator++();

        const K &operator++(int);

        int operator!();

        void step_fwd() {
            assert(LT->cursor_ok());
            LT->step_fwd();
        }
    };

    LookupTable();

    LookupTable(const LookupTable &source);

    LookupTable &operator=(const LookupTable &rhs);

    ~LookupTable();

    LookupTable &begin();

    int size() const;
    // PROMISES: Returns number of keys in the table.

    int cursor_ok() const;
    // PROMISES:
    //   Returns 1 if the cursor is attached to a key/datum pair,
    //   and 0 if the cursor is in the off-list state.

    const T &cursor_key() const;
    // REQUIRES: cursor_ok()
    // PROMISES: Returns key of key/datum pair to which cursor is attached.

    const K & cursor_datum() const;
    // REQUIRES: cursor_ok()
    // PROMISES: Returns datum of key/datum pair to which cursor is attached.

    void insert(const Pair<T, K> &pairA);
    // PROMISES:
    //   If keyA matches a key in the table, the datum for that
    //   key is set equal to datumA.
    //   If keyA does not match an existing key, keyA and datumM are
    //   used to create a new key/datum pair in the table.
    //   In either case, the cursor goes to the off-list state.

    void remove(const T &keyA);
    // PROMISES:
    //   If keyA matches a key in the table, the corresponding
    //   key/datum pair is removed from the table.
    //   If keyA does not match an existing key, the table is unchanged.
    //   In either case, the cursor goes to the off-list state.

    void find(const T &keyA);
    // PROMISES:
    //   If keyA matches a key in the table, the cursor is attached
    //   to the corresponding key/datum pair.
    //   If keyA does not match an existing key, the cursor is put in
    //   the off-list state.

    void go_to_first();
    // PROMISES: If size() > 0, cursor is moved to the first key/datum pair
    //   in the table.

    void step_fwd();
    // REQUIRES: cursor_ok()
    // PROMISES:
    //   If cursor is at the last key/datum pair in the list, cursor
    //   goes to the off-list state.
    //   Otherwise the cursor moves forward from one pair to the next.

    void make_empty();
    // PROMISES: size() == 0.


    friend ostream &operator<< <T, K>(ostream &os, const LookupTable<T, K> &lt);

private:
    int sizeM;
    LT_Node<T, K> *headM;
    LT_Node<T, K> *cursorM;

    void destroy();
    // Deallocate all nodes, set headM to zero.

    void copy(const LookupTable &source);
    // Establishes *this as a copy of source.  Cursor of *this will
    // point to the twin of whatever the source's cursor points to.
};



template<class T, class K>
LookupTable<T, K> &LookupTable<T, K>::begin() {
    cursorM = headM;
    return *this;
}

template<class T, class K>
LT_Node<T, K>::LT_Node(const Pair<T, K> &pairA, LT_Node<T, K> *nextA)
        : pairM(pairA), nextM(nextA) {
}

template<class T, class K>
LookupTable<T, K>::LookupTable()
        : sizeM(0), headM(0), cursorM(0) {
}

template<class T, class K>
LookupTable<T, K>::LookupTable(const LookupTable &source) {
    copy(source);
}

template<class T, class K>
LookupTable<T, K> &LookupTable<T, K>::operator=(const LookupTable &rhs) {
    if (this != &rhs) {
        destroy();
        copy(rhs);
    }
    return *this;
}

template<class T, class K>
LookupTable<T, K>::~LookupTable() {
    destroy();
}

template<class T, class K>
int LookupTable<T, K>::size() const {
    return sizeM;
}

template<class T, class K>
int LookupTable<T, K>::cursor_ok() const {
    return cursorM != 0;
}

template<class T, class K>
const T &LookupTable<T, K>::cursor_key() const {
    assert(cursor_ok());
    return cursorM->pairM.key;
}

template<class T, class K>
const K &LookupTable<T, K>::cursor_datum() const {
    assert(cursor_ok());
    return cursorM->pairM.datum;
}

template<class T, class K>
void LookupTable<T, K>::insert(const Pair<T, K> &pairA) {
    // Add new node at head?
    if (headM == 0 || pairA.key < headM->pairM.key) {
        headM = new LT_Node<T, K>(pairA, headM);
        sizeM++;
    }

        // Overwrite datum at head?
    else if (pairA.key == headM->pairM.key)
        headM->pairM.datum = pairA.datum;

        // Have to search ...

    else {
        LT_Node<T, K> *before = headM;
        LT_Node<T, K> *after = headM->nextM;

        while (after != NULL && (pairA.key > after->pairM.key)) {
            before = after;
            after = after->nextM;
        }

        if (after != NULL && pairA.key == after->pairM.key) {
            after->pairM.datum = pairA.datum;
        } else {
            before->nextM = new LT_Node<T, K>(pairA, before->nextM);
            sizeM++;
        }
    }
}

template<class T, class K>
void LookupTable<T, K>::remove(const T &keyA) {

    if (headM == 0 || keyA < headM->pairM.key)
        return;

    LT_Node<T, K> *doomed_node = 0;
    if (keyA == headM->pairM.key) {
        doomed_node = headM;
        headM = headM->nextM;
        sizeM--;
    } else {
        LT_Node<T, K> *before = headM;
        LT_Node<T, K> *maybe_doomed = headM->nextM;
        while (maybe_doomed != 0 && keyA > maybe_doomed->pairM.key) {
            before = maybe_doomed;
            maybe_doomed = maybe_doomed->nextM;
        }

        if (maybe_doomed != 0 && maybe_doomed->pairM.key == keyA) {
            doomed_node = maybe_doomed;
            before->nextM = maybe_doomed->nextM;
            sizeM--;
        }
    }
    delete doomed_node;           // Does nothing if doomed_node == 0.
}

template<class T, class K>
void LookupTable<T, K>::find(const T &keyA) {
    LT_Node<T, K> *ptr = headM;
    while (ptr != NULL && ptr->pairM.key != keyA) {
        ptr = ptr->nextM;
    }

    cursorM = ptr;
}

template<class T, class K>
void LookupTable<T, K>::go_to_first() {
    cursorM = headM;
}

template<class T, class K>
void LookupTable<T, K>::step_fwd() {
    assert(cursor_ok());
    cursorM = cursorM->nextM;
}

template<class T, class K>
void LookupTable<T, K>::make_empty() {
    destroy();
    sizeM = 0;
    cursorM = 0;
}

template<class T, class K>
void LookupTable<T, K>::destroy() {

    LT_Node<T, K> *ptr = headM;
    while (ptr != NULL) {
        headM = headM->nextM;
        delete ptr;
        ptr = headM;

    }
    cursorM = NULL;
    sizeM = 0;
}

template<class T, class K>
void LookupTable<T, K>::copy(const LookupTable &source) {

    headM = 0;
    cursorM = 0;

    if (source.headM == 0)
        return;

    for (LT_Node<T, K> *p = source.headM; p != 0; p = p->nextM) {
        insert(Pair<T, K>(p->pairM.key, p->pairM.datum));
        if (source.cursorM == p)
            find(p->pairM.key);
    }

}

template <class T, class K>
ostream &operator<<(ostream &os, const LookupTable<T, K> &lt) {
    if (lt.cursor_ok())
        os << lt.cursor_key() << "  " << lt.cursor_datum();
    else
        os << "Not Found.";

    return os;
}

template<class T, class K>
const K &LookupTable<T, K>::Iterator::operator*() {
    assert(LT->cursor_ok());
    return LT->cursor_datum();
}

template<class T, class K>
const K &LookupTable<T, K>::Iterator::operator++() {
    assert(LT->cursor_ok());
    const K &x = LT->cursor_datum();
    LT->step_fwd();
    return x;
}

template<class T, class K>
const K &LookupTable<T, K>::Iterator::operator++(int) {
    assert(LT->cursor_ok());

    LT->step_fwd();
    return LT->cursor_datum();
}

template<class T, class K>
int LookupTable<T, K>::Iterator::operator!() {
    return (LT->cursor_ok());
}

#endif
