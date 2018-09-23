#include <iostream>
#include <fstream>

std::ifstream *inFile;
std::ofstream *outFile1;
std::ofstream *outFile2;

class DijktraSSS {
private:
    int numNodes;
    int sourceNode = 1;
    int minNode;
    int currentNode;
    int newCost;

    int **costMatrix;
    int *fatherAry;
    int *markedAry;
    int *bestCostAry;

public:
    DijktraSSS(int numNodes) {
        this->numNodes = numNodes;

        costMatrix = new int *[numNodes + 1];
        for (int i = 1; i <= numNodes; i++) {
            costMatrix[i] = new int[numNodes + 1];
            for (int x = 1; x <= numNodes; x++) {
                costMatrix[i][x] = 99999;
            }

            costMatrix[i][i] = 0;
        }

        fatherAry = new int[numNodes + 1];
        markedAry = new int[numNodes + 1];
        bestCostAry = new int[numNodes + 1];

        for (int i = 1; i <= numNodes; i++) {
            fatherAry[i] = i;
            markedAry[i] = 0;
            bestCostAry[i] = 99999;
        }

        loadCostMatrix();

        while (sourceNode <= numNodes) {
            setBestCostAry();
            setFatherAry();
            setMarkedAry();

            while (!allNodesMarked()) {
                int minCost = 999999;
                for (int i = 1; i <= numNodes; ++i) {
                    if (markedAry[i] == 0 && bestCostAry[i] < minCost) {
                        minNode = i;
                        minCost = bestCostAry[i];
                    }
                }
                markMinNode();

                debugPrint();

                for (int i = 1; i <= numNodes; ++i) {
                    if (markedAry[i] == 0) {
                        currentNode = i;

                        newCost = computeCost();

                        if (newCost < bestCostAry[currentNode]) {
                            changeFather();
                            changeCost();
                        }
                    }
                }
            }

            *outFile1 << "Source node  = " << sourceNode << " \n"
                      << "The shortest paths  from the source node " << sourceNode << " are:\n";

            printShortestPaths();
            sourceNode++;
        }
    }

    bool allNodesMarked() {
        for (int i = 1; i <= numNodes; ++i) {
            if (markedAry[i] == 0) {
                return false;
            }
        }

        return true;
    }

    void loadCostMatrix() {
        int a, b, c;
        while (*inFile >> a && *inFile >> b && *inFile >> c) {
            costMatrix[a][b] = c;
        }
    }

    void setBestCostAry() {
        bestCostAry = costMatrix[sourceNode];
    }

    void setFatherAry() {
        for (int i = 1; i <= numNodes; ++i) {
            fatherAry[i] = sourceNode;
        }
    }

    void setMarkedAry() {
        for (int i = 1; i <= numNodes; ++i) {
            markedAry[i] = 0;
        }
    }

    int computeCost() {
        return bestCostAry[minNode] + costMatrix[minNode][currentNode];
    }

    void markMinNode() {
        markedAry[minNode] = 1;
    }

    void changeFather() {
        fatherAry[currentNode] = minNode;
    }

    void changeCost() {
        bestCostAry[currentNode] = newCost;
    }

    void debugPrint() {
        *outFile2 << "The sourceNode is: " << sourceNode << '\n';
        *outFile2 << "The fatherAry is: ";
        for (int i = 1; i <= numNodes - 1; ++i) {
            *outFile2 << fatherAry[i] << ",";
        }
        *outFile2 << fatherAry[numNodes - 1] << "\n";

        *outFile2 << "The bestCostAry is: ";
        for (int i = 1; i <= numNodes - 1; ++i) {
            *outFile2 << bestCostAry[i] << ",";
        }
        *outFile2 << bestCostAry[numNodes - 1] << "\n";

        *outFile2 << "The markedAry is: ";
        for (int i = 1; i <= numNodes - 1; ++i) {
            *outFile2 << markedAry[i] << ",";
        }
        *outFile2 << markedAry[numNodes - 1] << "\n";
    }

    void printShortestPaths() {
        for (int i = 1; i <= numNodes; i++) {
            *outFile1 << "The path from " << sourceNode << " to " << i << ": ";

            int x = i;
            *outFile1 << x;
            while (fatherAry[x] != sourceNode) {
                x = fatherAry[x];
                *outFile1 << " <- " << x;
            }
            *outFile1 << " <- " << sourceNode << "   Cost = " << bestCostAry[i] << '\n';
        }

        *outFile1 << "\n===============================\n";
    }
};

int main(int argc, char *argv[]) {
    inFile = new std::ifstream(argv[1]);
    outFile1 = new std::ofstream(argv[2]);
    outFile2 = new std::ofstream(argv[3]);

    int numNodes;
    *inFile >> numNodes;
    *outFile1 << "==============================\n"
              << "There are " << numNodes << " nodes in the input graph.\n" <<
              "===============================\n";

    DijktraSSS dijktraSSS(numNodes);

    inFile->close();
    outFile1->close();
    outFile2->close();
}