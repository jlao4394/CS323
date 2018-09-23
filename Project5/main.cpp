#include <iostream>
#include <fstream>

std::ifstream *inFile1;
std::ifstream *inFile2;
std::ofstream *outFile;

class Node {
public:
    int jobId;
    int jobTime;

    Node *next = nullptr;

    Node(int jobId, int jobTime) {
        this->jobId = jobId;
        this->jobTime = jobTime;
    }
};

class Scheduler {
public:
    int numNodes;
    int numProc;
    int totalJobTimes;
    int currentTime = 0;
    int procUsed = 0;
    int **adjacencyMatrix;
    int **scheduleTable;
    int *jobTimeAry;
    Node *open = nullptr;
    int *processJob;
    int *processTime;
    int *parentCount;
    int *jobDone;
    int *jobMarked;

    Scheduler(int numNodes, int numProc) {
        this->numNodes = numNodes;
        this->numProc = numProc;

        adjacencyMatrix = new int *[numNodes + 1];
        for (int i = 0; i < numNodes + 1; i++) {
            adjacencyMatrix[i] = new int[numNodes + 1]{0};
        }

        jobTimeAry = new int[numNodes + 1]{0};
        processJob = new int[numProc + 1]{0};
        processTime = new int[numProc + 1]{0};
        parentCount = new int[numNodes + 1]{0};
        jobDone = new int[numNodes + 1]{0};
        jobMarked = new int[numNodes + 1]{0};
    }

    void loadMatrix() {
        int i;
        int j;

        while (*inFile1 >> i && *inFile1 >> j) {
            adjacencyMatrix[i][j]++;
            parentCount[j]++;
        }
    }

    void computeTotalJobTimes() {
        int i;
        int j;
        int sum = 0;

        *inFile2 >> i;
        while (*inFile2 >> i && *inFile2 >> j) {
            jobTimeAry[i] = j;
            sum += j;
        }

        totalJobTimes = sum;

        scheduleTable = new int *[numProc + 1];
        for (i = 0; i < numProc + 1; i++) {
            scheduleTable[i] = new int[totalJobTimes + 1]{0};
        }
    }

    Node *getUnmarkedOrphan() {
        for (int i = 1; i < numNodes + 1; i++) {
            if (jobMarked[i] == 0 && parentCount[i] == 0) {
                jobMarked[i] = 1;
                return new Node(i, jobTimeAry[i]);
            }
        }

        return nullptr;
    }

    void insertOpen(Node *node) {
        if (open == nullptr) {
            open = node;
            return;
        }

        Node *n = open;
        while (n->jobTime < node->jobTime && n->next != nullptr) {
            n = n->next;
        }

        node->next = n->next;
        n->next = node;
    }

    Node *popOpen() {
        Node *node = open;
        open = open->next;
        return node;
    }

    void printList() {
        Node *node = open;

        std::cout << "Open";
        while (node != nullptr) {
            std::cout << " -> (" << node->jobId << ", " << node->jobTime << ")";
            node = node->next;
        }

        std::cout << " -> NULL\n";
    }

    void printTable() {
        *outFile << "     ";
        for (int i = 0; i < currentTime; i++) {
            if (i < 10) {
                *outFile << "--" << i << "--";
            } else {
                *outFile << "-" << i << "-";
            }
        }

        *outFile << "-\n";

        for (int i = 1; i < numProc + 1; i++) {
            *outFile << "P(" << i << ")";
            if (i < 10) {
                *outFile << " ";
            }

            for (int j = 0; j < currentTime; j++) {
                if (scheduleTable[i][j] > 0) {
                    *outFile << "| " << scheduleTable[i][j] << " ";
                } else {
                    *outFile << "| " << "-" << " ";
                }

                if (scheduleTable[i][j] < 10) {
                    *outFile << " ";
                }
            }

            *outFile << "|\n";
        }

        *outFile << "\n\n";
    }

    int findProcessor() {
        for (int i = 1; i < numProc + 1; i++) {
            if (processTime[i] <= 0) {
                return i;
            }
        }

        return -1;
    }

    void populateTable() {
        while (open != nullptr && procUsed < numProc) {
            int availProc = findProcessor();
            if (availProc > 0) {
                procUsed++;
                Node *newJob = popOpen();
                processJob[availProc] = newJob->jobId;
                processTime[availProc] = newJob->jobTime;
                updateTable(availProc, newJob);
            }
        }
    }

    void updateTable(int proc, Node *job) {
        for (int i = currentTime; i <= currentTime + job->jobTime; i++) {
            scheduleTable[proc][i] = job->jobId;
        }
    }

    void incrementTime() {
        currentTime++;
        for (int i = 1; i < numProc + 1; i++) {
            processTime[i]--;
        }

        int jobId;
        while ((jobId = findDoneJob()) != -1) {
            procUsed--;
            deleteEdge(jobId);
            deleteNode(jobId);
        }
    }

    bool graphEmpty() {
        for (int i = 1; i < numNodes + 1; i++) {
            if (jobDone[i] == 0) {
                return false;
            }
        }

        return true;
    }

    bool checkCycle() {
        bool procFinished = true;
        for (int i = 1; i < numProc + 1; i++) {
            if (processTime[i] > 0) {
                procFinished = false;
            }
        }

        return open == nullptr && !graphEmpty() && procFinished;
    }

    int findDoneJob() {
        for (int i = 1; i < numProc + 1; i++) {
            if (processTime[i] <= 0 && processJob[i] != 0) {
                int jobId = processJob[i];
                processJob[i] = 0;
                return jobId;
            }
        }

        return -1;
    }

    void deleteNode(int jobId) {
        jobDone[jobId] = 1;
    }

    void deleteEdge(int jobId) {
        for (int i = 1; i < numNodes + 1; i++) {
            if (adjacencyMatrix[jobId][i] > 0) {
                parentCount[i]--;
            }
        }
    }
};

int main(int argc, char *argv[]) {
    inFile1 = new std::ifstream(argv[1]);
    inFile2 = new std::ifstream(argv[2]);
    outFile = new std::ofstream(argv[3]);

    int numNodes;
    *inFile1 >> numNodes;

    int numProc = 0;

    while (numProc <= 0) {
        std::cout << "Number of processors:";
        std::cin >> numProc;
        if (numProc <= 0) {
            std::cout << "Invalid number of processors.";
        }
    }

    if (numProc > numNodes) {
        numProc = numNodes;
    }

    Scheduler scheduler(numNodes, numProc);
    scheduler.loadMatrix();
    scheduler.computeTotalJobTimes();

    while (!scheduler.graphEmpty()) {
        Node *orphan;

        while ((orphan = scheduler.getUnmarkedOrphan()) != nullptr) {
            scheduler.insertOpen(orphan);
        }

        scheduler.printList();

        scheduler.populateTable();

        if (scheduler.checkCycle()) {
            scheduler.printTable();

            std::cout << "Cycle in the graph!";
            *outFile << "Cycle in the graph!";

            inFile1->close();
            inFile2->close();
            outFile->close();
            return 0;
        }

        scheduler.printTable();

        scheduler.incrementTime();

        std::cout << "Current Time: " << scheduler.currentTime << "\n";
        std::cout << "jobMarked: [";
        for (int i = 1; i < numNodes + 1; i++) {
            std::cout << scheduler.jobMarked[i];
            if (i < numNodes) {
                std::cout << ", ";
            }
        }
        std::cout << "]\n";
        std::cout << "processTime: [";
        for (int i = 1; i < numProc + 1; i++) {
            std::cout << scheduler.processTime[i];
            if (i < numProc) {
                std::cout << ", ";
            }
        }
        std::cout << "]\n";
        std::cout << "processJob: [";
        for (int i = 1; i < numProc + 1; i++) {
            std::cout << scheduler.processJob[i];
            if (i < numProc) {
                std::cout << ", ";
            }
        }
        std::cout << "]\n";
        std::cout << "jobDone: [";
        for (int i = 1; i < numNodes + 1; i++) {
            std::cout << scheduler.jobDone[i];
            if (i < numNodes) {
                std::cout << ", ";
            }
        }
        std::cout << "]\n\n";
    }

    scheduler.printTable();

    inFile1->close();
    inFile2->close();
    outFile->close();

    return 0;
}

