import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

public class Project2 {
    private static Scanner inFile;
    private static FileWriter outFile1;
    private static FileWriter outFile2;
    private static FileWriter outFile3;
    private static FileWriter outFile4;

    private static class ListBinTreeNode {
        String chStr;
        int prob;
        String code;
        ListBinTreeNode next;
        ListBinTreeNode left;
        ListBinTreeNode right;

        ListBinTreeNode(String chStr, int prob) {
            this.chStr = chStr;
            this.prob = prob;
        }

        ListBinTreeNode() {
        }

        void printNode(FileWriter outFile) {
            try {
                outFile.append("(" + chStr + ", " + prob + ", " + (code != null ? code : "NULL") + ", " + (next != null ? next.chStr : "NULL") + ", " + (left != null ? left.chStr : "NULL") + ", " + (right != null ? right.chStr : "NULL") + ")\n");
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    private static class HuffmanLListTree {
        ListBinTreeNode listHead;
        ListBinTreeNode root;

        HuffmanLListTree() {
            this.listHead = new ListBinTreeNode();
        }

        void constructLinkedList() {
            String chStr;
            int prob;

            while (inFile.hasNext()) {
                chStr = inFile.next();
                prob = inFile.nextInt();

                ListBinTreeNode spot = findSpot(prob);
                ListBinTreeNode node = new ListBinTreeNode(chStr, prob);
                insertNewNode(spot, node);

                printList();
            }
        }

        void constructBinTree() {
            while (listHead.next.next != null) {
                ListBinTreeNode first = listHead.next;
                ListBinTreeNode second = first.next;

                ListBinTreeNode node = new ListBinTreeNode();
                node.chStr = first.chStr + second.chStr;
                node.prob = first.prob + second.prob;
                node.left = first;
                node.right = second;

                node.printNode(outFile2);

                ListBinTreeNode spot = findSpot(node.prob);
                insertNewNode(spot, node);

                listHead = listHead.next.next;

                printList();
            }

            root = listHead.next;
        }

        void constructCode(ListBinTreeNode node, String s) {
            if (node == null) {
                return;
            } else if (isLeaf(node)) {
                node.code = s;

                try {
                    outFile3.append("(" + node.chStr + ", " + node.code + ")\n");
                } catch (IOException e) {
                    e.printStackTrace();
                }
            } else {
                constructCode(node.left, s + "0");
                constructCode(node.right, s + "1");
            }
        }

        void preOrder(ListBinTreeNode node) {
            if (node == null) {
                return;
            }

            node.printNode(outFile4);
            preOrder(node.left);
            preOrder(node.right);
        }

        void inOrder(ListBinTreeNode node) {
            if (node == null) {
                return;
            }

            inOrder(node.left);
            node.printNode(outFile4);
            inOrder(node.right);
        }

        void postOrder(ListBinTreeNode node) {
            if (node == null) {
                return;
            }

            postOrder(node.left);
            postOrder(node.right);
            node.printNode(outFile4);
        }

        ListBinTreeNode findSpot(int prob) {
            ListBinTreeNode node = listHead;
            while (node.next != null && node.next.prob < prob) {
                node = node.next;
            }

            return node;
        }

        void insertNewNode(ListBinTreeNode spot, ListBinTreeNode node) {
            node.next = spot.next;
            spot.next = node;
        }

        boolean isLeaf(ListBinTreeNode node) {
            return node.left == null && node.right == null;
        }

        void printList() {
            String string = "listHead";

            ListBinTreeNode node = listHead;

            while ((node = node.next) != null) {
                string += " --> (\"" + node.chStr + "\", " + node.prob + ", " + (node.next != null ? "\"" + node.next.chStr + "\"" : "NULL") + ")";
            }

            string += " --> NULL\n";

            try {
                outFile1.append(string);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    public static void main(String... args) {
        try {
            inFile = new Scanner(new File(args[0]));
            outFile1 = new FileWriter(new File(args[1]));
            outFile2 = new FileWriter(new File(args[2]));
            outFile3 = new FileWriter(new File(args[3]));
            outFile4 = new FileWriter(new File(args[4]));
        } catch (IOException e) {
            e.printStackTrace();
        }

        HuffmanLListTree huffmanLListTree = new HuffmanLListTree();
        huffmanLListTree.constructLinkedList();
        huffmanLListTree.constructBinTree();

        if (huffmanLListTree.root == null) {
            System.out.print("this is an empty tree");
            return;
        }

        huffmanLListTree.constructCode(huffmanLListTree.root, "");

        try {
            outFile4.append("Preorder Traversal:\n");
            huffmanLListTree.preOrder(huffmanLListTree.root);
            outFile4.append("\n\nInorder Traversal:\n");
            huffmanLListTree.inOrder(huffmanLListTree.root);
            outFile4.append("\n\nPostorder Traversal:\n");
            huffmanLListTree.postOrder(huffmanLListTree.root);
        } catch (IOException e) {
            e.printStackTrace();
        }

        inFile.close();
        try {
            outFile1.close();
            outFile2.close();
            outFile3.close();
            outFile4.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
