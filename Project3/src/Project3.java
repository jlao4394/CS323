import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

public class Project3 {
    private static class HeapSort {
        int rootIndex;
        int numItems;
        int[] heapAry;

        int countData() {
            int count = 0;
            while (inFile.hasNextInt()) {
                count++;
                inFile.nextInt();
            }

            return count;
        }

        void buildHeap() {
            rootIndex = 1;

            int data;
            int kidIndex;
            while (inFile.hasNextInt()) {
                data = inFile.nextInt();
                insertOneDataItem(data);
                kidIndex = heapAry[0];
                bubbleUp(kidIndex);
                printHeap();
            }
        }

        void deleteHeap() {
            while (!isHeapEmpty()) {
                replaceRoot();
            }
        }

        void insertOneDataItem(int data) {
            heapAry[0]++;
            heapAry[heapAry[0]] = data;
        }

        int getRoot() {
            return heapAry[1];
        }

        void replaceRoot() {
            int data = getRoot();

            try {
                outFile2.append(data + "\n");
            } catch (IOException e) {
                e.printStackTrace();
            }

            heapAry[1] = heapAry[heapAry[0]];
            heapAry[0]--;

            bubbleDown(rootIndex);

            printHeap();

            if (!isHeapEmpty()) {
                replaceRoot();
            }
        }

        void bubbleUp(int kidIndex) {
            if (isRoot(kidIndex)) {
                return;
            } else {
                int fatherIndex = kidIndex / 2;

                if (heapAry[kidIndex] >= heapAry[fatherIndex]) {
                    return;
                } else {
                    int temp = heapAry[fatherIndex];
                    heapAry[fatherIndex] = heapAry[kidIndex];
                    heapAry[kidIndex] = temp;
                    bubbleUp(fatherIndex);
                }
            }
        }

        void bubbleDown(int fatherIndex) {
            if (isLeaf(fatherIndex)) {
                return;
            } else {
                int leftKidIndex = fatherIndex * 2;
                int rightKidIndex = fatherIndex * 2 + 1;

                int minKidIndex = findMinKidIndex(leftKidIndex, rightKidIndex);

                if (heapAry[minKidIndex] >= heapAry[fatherIndex]) {
                    return;
                } else {
                    int temp = heapAry[fatherIndex];
                    heapAry[fatherIndex] = heapAry[minKidIndex];
                    heapAry[minKidIndex] = temp;

                    bubbleDown(minKidIndex);
                }
            }
        }

        boolean isLeaf(int index) {
            return index * 2 > heapAry[0];
        }

        boolean isRoot(int index) {
            return index == 1;
        }

        int findMinKidIndex(int leftKidIndex, int rightKidIndex) {
            if (rightKidIndex > heapAry[0]) {
                return leftKidIndex;
            }

            if (heapAry[leftKidIndex] <= heapAry[rightKidIndex]) {
                return leftKidIndex;
            } else {
                return rightKidIndex;
            }
        }

        boolean isHeapEmpty() {
            return heapAry[0] == 0;
        }

        boolean isHeapFull() {
            return heapAry[0] == numItems;
        }

        void printHeap() {
            String output = "[";
            for (int i = 0; i <= heapAry[0]; i++) {
                output += heapAry[i];

                if (i < heapAry[0]) {
                    output += ", ";
                }
            }

            output += "]\n";

            try {
                outFile1.append(output);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    private static Scanner inFile;
    private static FileWriter outFile1;
    private static FileWriter outFile2;

    public static void main(String[] args) {
        try {
            inFile = new Scanner(new File(args[0]));
            outFile1 = new FileWriter(new File(args[1]));
            outFile2 = new FileWriter(new File(args[2]));
        } catch (java.io.IOException e) {
            e.printStackTrace();
        }

        HeapSort heapSort = new HeapSort();
        heapSort.numItems = heapSort.countData();
        heapSort.heapAry = new int[heapSort.numItems + 1];
        heapSort.heapAry[0] = 0;

        inFile.close();
        try {
            inFile = new Scanner(new File(args[0]));
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }

        heapSort.buildHeap();
        heapSort.deleteHeap();

        inFile.close();
        try {
            outFile1.close();
            outFile2.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
