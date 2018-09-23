#include <iostream>
#include <fstream>

std::ifstream *inFile;
std::ofstream *outFile1;
std::ofstream *outFile2;

class ListNode {
    friend class LinkedListStack;

    friend class LinkedListQueue;

    friend class RadixSort;

public:
    int data;
    ListNode *next = nullptr;

    ListNode() {
    }

    ListNode(int data) {
        this->data = data;
    }
};

class LinkedListStack {
    friend class RadixSort;

private:
    ListNode *top = nullptr;
public:
    void push(ListNode *node) {
        node->next = top;
        top = node;
    }

    ListNode *pop() {
        ListNode *temp = top;
        top = top->next;

        temp->next = nullptr;
        return temp;
    }

    bool isEmpty() {
        return top == nullptr;
    }

    void printStack() {
        ListNode *node = top;

        *outFile2 << "Top";
        while ((node = node->next)) {
            *outFile2 << " -> (" << node->data << ", ";
            if (node->next) {
                *outFile2 << node->next->data;
            } else {
                *outFile2 << "NULL";
            }
            *outFile2 << ")";
        }

        *outFile2 << " -> NULL\n\n";
    }
};

class LinkedListQueue {
    friend class RadixSort;

private:
    ListNode *head = nullptr;
    ListNode *tail = nullptr;

public:
    LinkedListQueue() {
        head = new ListNode();
        tail = head;
    }

    void addTail(ListNode *node) {
        tail->next = node;
        tail = node;
    }

    ListNode *deleteHead() {
        ListNode *temp = head->next;
        head->next = head->next->next;
        if (tail == temp) {
            tail = head;
        }

        temp->next = nullptr;
        return temp;
    }

    bool isEmpty() {
        return head->next == nullptr;
    }

    void printQueue() {
        ListNode *node = head->next;

        *outFile2 << "Front (index)";

        while (node) {
            *outFile2 << " -> (" << node->data << ", ";
            if (node->next) {
                *outFile2 << node->next->data;
            } else {
                *outFile2 << "NULL";
            }

            *outFile2 << ")";

            node = node->next;
        }

        *outFile2 << " -> NULL\n";

        node = tail;

        *outFile2 << "Tail (index)";

        while (node) {
            *outFile2 << " -> (" << node->data << ", ";
            if (node->next) {
                *outFile2 << node->next->data;
            } else {
                *outFile2 << "NULL";
            }

            *outFile2 << ")";

            node = node->next;
        }

        *outFile2 << " -> NULL\n";
    }
};

class RadixSort {
private:
    const int tableSize = 10;
    LinkedListQueue ***hashTable;
    LinkedListStack *stack;
    int previousTable;
    int maxDigits;
    int offSet;
    int currentDigit = 0;
    int currentTable = 0;

public:
    RadixSort() {
        hashTable = new LinkedListQueue **[2];
        for (int i = 0; i < 2; i++) {
            hashTable[i] = new LinkedListQueue *[tableSize];

            for (int j = 0; j < tableSize; j++) {
                hashTable[i][j] = new LinkedListQueue();
            }
        }

        stack = new LinkedListStack();
    }

    void firstReading() {
        int negativeNumber = 0;
        int positiveNumber = 0;
        int data;
        while (*inFile >> data) {
            if (data < negativeNumber) {
                negativeNumber = data;
            } else if (data > positiveNumber) {
                positiveNumber = data;
            }
        }

        offSet = abs(negativeNumber);
        positiveNumber += offSet;
        maxDigits = getMaxDigits(positiveNumber);
    }

    void loadStack() {
        int data;

        while (*inFile >> data) {
            data += offSet;
            ListNode *newNode = new ListNode(data);

            stack->push(newNode);
        }

        *outFile2 << "*** Below is the constructed stack ***\n";
        stack->printStack();
    }

    void dumpStack() {
        while (!stack->isEmpty()) {
            ListNode *node = stack->pop();

            int digit = getDigit(node->data);

            hashTable[currentTable][digit]->addTail(node);
        }

        printTable(hashTable[currentTable]);
    }

    int getMaxDigits(int i) {
        int temp = i;
        int digits = 0;
        while ((temp % 10) != 0) {
            digits++;
            temp /= 10;
        }

        return digits;
    }

    int getDigit(int data) {
        int temp = data;
        for (int i = 0; i < currentDigit; i++) {
            temp /= 10;
        }

        return temp % 10;
    }

    void radixSort() {
        dumpStack();

        currentDigit++;
        currentTable = 1;
        previousTable = 0;
        int currentQueue = 0;

        while (currentDigit <= maxDigits) {
            while (currentQueue < tableSize) {
                while (!hashTable[previousTable][currentQueue]->isEmpty()) {
                    ListNode *node = hashTable[previousTable][currentQueue]->deleteHead();
                    node->next = nullptr;
                    int digit = getDigit(node->data);

                    hashTable[currentTable][digit]->addTail(node);
                }

                currentQueue++;
            }

            int temp = currentTable;
            currentTable = previousTable;
            previousTable = temp;
            currentQueue = 0;
            currentDigit++;
        }

        printTable(hashTable[previousTable]);
    }

    void printTable(LinkedListQueue **table) {
        for (int i = 0; i < tableSize; i++) {
            if (table[i]->isEmpty()) {
                continue;
            }

            *outFile2 << "Table[" << currentTable << "][" << i << "]: ";

            ListNode *node = table[i]->head->next;

            while (node) {
                *outFile2 << node->data;
                if (node->next) {
                    *outFile2 << ", ";
                }

                node = node->next;
            }

            *outFile2 << "\n";
        }

        *outFile2 << "\n";
    }

    void printResults() {
        ListNode *node = hashTable[previousTable][0]->head->next;

        while (node) {
            *outFile1 << (node->data - offSet) << " ";

            node = node->next;
        }
    }
};

int main(int argc, char *argv[]) {
    inFile = new std::ifstream(argv[1]);
    outFile1 = new std::ofstream(argv[2]);
    outFile2 = new std::ofstream(argv[3]);

    RadixSort radixSort;
    radixSort.firstReading();

    inFile->close();
    inFile = new std::ifstream(argv[1]);

    radixSort.loadStack();
    radixSort.radixSort();
    radixSort.printResults();

    outFile1->close();
    outFile2->close();

    return 0;
}

