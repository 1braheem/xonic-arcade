#ifndef MINHEAP_H
#define MINHEAP_H

#include "Player.h"

class MinHeap {
private:
    Player* heap;
    int capacity;
    int size;

public:

    MinHeap() {
        capacity = 11;
        size = 0;
        heap = new Player[capacity];
    }


    ~MinHeap() {
        delete[] heap;
    }

    void insertIntoMinHeap(Player val) {
        if (size == capacity) {
            cout << "Heap is full" << endl;
            return;
        }
        size++;
        heap[size] = val;
        int i = size;

        while (i > 1 && heap[i].getScore() < heap[i / 2].getScore()) {
            swap(heap[i], heap[i / 2]);
            i = i / 2;
        }
    }
    void deleteFromMinHeap() {
        if (size == 0) {
            cout << "There is no element in heap\n";
            return;
        }
        heap[0] = heap[1];
        heap[1] = heap[size];
        heap[size] = heap[0];
        size--;

        int i = 1;

        while (i <= size) {
            int left = 2 * i;
            int right = 2 * i + 1;
            int smallest = i;

            if (left <= size && heap[left].getScore() < heap[smallest].getScore()) {
                smallest = left;
            }
            if (right <= size && heap[right].getScore() < heap[smallest].getScore()) {
                smallest = right;
            }
            if (smallest != i) {
                swap(heap[i], heap[smallest]);
                i = smallest;
            }
            else {
                break;
            }
        }
    }
    Player* getHeapArray() {
        return heap;
    }
};

#endif