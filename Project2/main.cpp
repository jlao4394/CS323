#include <iostream>
#include <fstream>

std::ifstream *inFile = nullptr;
std::ofstream *outFile1 = nullptr;
std::ofstream *outFile2 = nullptr;
std::ofstream *outFile3 = nullptr;
std::ofstream *outFile4 = nullptr;

class ListBinTreeNode {
public:
    std::string chString;
    int prob;
    std::string code = "NULL";
    ListBinTreeNode *next = nullptr;
    ListBinTreeNode *left = nullptr;
    ListBinTreeNode *right = nullptr;

    ListBinTreeNode(std::string chString, int prob) {
        this->chString = chString;
        this->prob = prob;
    }

    ListBinTreeNode() = default;

    void printNode(std::ofstream *outFile) {
        *outFile << "(" << chString << ", " << prob << ", " << code << ", " << (next ? next->chString : "NULL") << ", "
                  << (left ? left->chString : "NULL") << ", " << (right ? right->chString : "NULL") << ")" << std::endl;
    }
};

class HuffmanLListTree {
private:
    ListBinTreeNode *listHead = nullptr;

public:
    ListBinTreeNode *root = nullptr;

    HuffmanLListTree() {
        listHead = new ListBinTreeNode();
    }

    void constructLinkedListTree() {
        std::string chString;
        int prob;
        ListBinTreeNode *spot;
        ListBinTreeNode *node;

        while (*inFile >> chString && *inFile >> prob) {
            spot = findSpot(prob);
            node = new ListBinTreeNode(chString, prob);
            insertNewNode(spot, node);
            printList();
        }
    }

    void constructBinTree() {
        ListBinTreeNode *first;
        ListBinTreeNode *second;
        while (listHead->next->next) {
            first = listHead->next;
            second = first->next;
            ListBinTreeNode *node = new ListBinTreeNode(first->chString + second->chString, first->prob + second->prob);
            node->left = first;
            node->right = second;
            node->printNode(outFile2);

            ListBinTreeNode *spot = findSpot(node->prob);
            insertNewNode(spot, node);
            listHead = listHead->next->next;
            printList();
        }

        root = listHead->next;
    }

    void constructCode(ListBinTreeNode *node, std::string string) {
        if (!node) {
            return;
        } else if (isLeaf(node)) {
            node->code = string;
            *outFile3 << "(" << node->chString << ", " << node->code << ")" << std::endl;
        } else {
            constructCode(node->left, string + "0");
            constructCode(node->right, string + "1");
        }
    }

    void preOrder(ListBinTreeNode *node) {
        if (!node) {
            return;
        } else {
            node->printNode(outFile4);
            preOrder(node->left);
            preOrder(node->right);
        }
    }

    void inOrder(ListBinTreeNode *node) {
        if (!node) {
            return;
        } else {
            inOrder(node->left);
            node->printNode(outFile4);
            inOrder(node->right);
        }
    }

    void postOrder(ListBinTreeNode *node) {
        if (!node) {
            return;
        } else {
            postOrder(node->left);
            postOrder(node->right);
            node->printNode(outFile4);
        }
    }

    ListBinTreeNode *findSpot(int prob) {
        ListBinTreeNode *node = listHead;

        while (node->next && node->next->prob < prob) {
            node = node->next;
        }

        return node;
    }

    void insertNewNode(ListBinTreeNode *spot, ListBinTreeNode *node) {
        node->next = spot->next;
        spot->next = node;
    }

    bool isLeaf(ListBinTreeNode *node) {
        return !(node->left) && !(node->right);
    }

    void printList() {
        *outFile1 << "listHead";

        ListBinTreeNode *node = listHead->next;

        while (node) {
            *outFile1 << " --> (" << "\"" + node->chString + "\"" << ", " << node->prob << ", "
                      << (node->next ? "\"" + node->next->chString + "\"" : "NULL") << ")";
            node = node->next;
        }

        *outFile1 << " --> NULL" << std::endl;
    }
};

int main(int argc, char *argv[]) {
    inFile = new std::ifstream(argv[1]);
    outFile1 = new std::ofstream(argv[2]);
    outFile2 = new std::ofstream(argv[3]);
    outFile3 = new std::ofstream(argv[4]);
    outFile4 = new std::ofstream(argv[5]);

    HuffmanLListTree huffmanLListTree;

    huffmanLListTree.constructLinkedListTree();
    huffmanLListTree.constructBinTree();

    if (huffmanLListTree.root) {
        huffmanLListTree.constructCode(huffmanLListTree.root, "");

        *outFile4 << "Preorder Traversal: " << std::endl;
        huffmanLListTree.preOrder(huffmanLListTree.root);

        *outFile4 << std::endl << std::endl << "Inorder Traversal: " << std::endl;
        huffmanLListTree.inOrder(huffmanLListTree.root);

        *outFile4 << std::endl << std::endl << "Postorder Traversal: " << std::endl;
        huffmanLListTree.postOrder(huffmanLListTree.root);
    } else {
        std::cout << "this is an empty tree";
    }

    inFile->close();
    outFile1->close();
    outFile2->close();
    outFile3->close();
    outFile4->close();

    return 0;
}

