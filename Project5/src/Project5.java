import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

public class Project5 {
    private static Scanner inFile1;
    private static Scanner inFile2;
    private static FileWriter outFile;

    static class Node {
        int jobId;
        int jobTime;

        Node next;

        Node(int jobId, int jobTime) {
            this.jobId = jobId;
            this.jobTime = jobTime;
        }
    }

    static class Scheduler {
        int numNodes;
        int numProc;
        int totalJobTimes;
        int currentTime = 0;
        int procUsed = 0;
        int adjacencyMatrix[][];
        int scheduleTable[][];
        int jobTimeAry[];
        Node open;
        int processJob[];
        int processTime[];
        int parentCount[];
        int jobDone[];
        int jobMarked[];

        Scheduler(int numNodes, int numProc) {
            this.numNodes = numNodes;
            this.numProc = numProc;

            adjacencyMatrix = new int[numNodes + 1][numNodes + 1];

            jobTimeAry = new int[numNodes + 1];
            processJob = new int[numProc + 1];
            processTime = new int[numProc + 1];
            parentCount = new int[numNodes + 1];
            jobDone = new int[numNodes + 1];
            jobMarked = new int[numNodes + 1];
        }

        void loadMatrix() {
            int i;
            int j;

            while (inFile1.hasNextInt()) {
                i = inFile1.nextInt();
                j = inFile1.nextInt();
                adjacencyMatrix[i][j]++;
                parentCount[j]++;
            }
        }

        void computeTotalJobTimes() {
            int i;
            int j;
            int sum = 0;

            inFile2.nextInt();
            while (inFile2.hasNextInt()) {
                i = inFile2.nextInt();
                j = inFile2.nextInt();
                jobTimeAry[i] = j;
                sum += j;
            }

            totalJobTimes = sum;

            scheduleTable = new int[numProc + 1][totalJobTimes + 1];
        }

        Node getUnmarkedOrphan() {
            for (int i = 1; i < numNodes + 1; i++) {
                if (jobMarked[i] == 0 && parentCount[i] == 0) {
                    jobMarked[i] = 1;
                    return new Node(i, jobTimeAry[i]);
                }
            }

            return null;
        }

        void insertOpen(Node node) {
            if (open == null) {
                open = node;
                return;
            }

            Node n = open;
            while (n.jobTime < node.jobTime && n.next != null) {
                n = n.next;
            }

            node.next = n.next;
            n.next = node;
        }

        Node popOpen() {
            Node node = open;
            open = open.next;
            return node;
        }

        void printList() throws IOException {
            Node node = open;

            System.out.print("Open");
            while (node != null) {
                System.out.print(" -> (" + node.jobId + ", " + node.jobTime + ")");
                node = node.next;
            }

            System.out.print(" -> NULL\n");
        }

        void printTable() throws IOException {
            outFile.append("     ");
            for (int i = 0; i < currentTime; i++) {
                if (i < 10) {
                    outFile.append("--" + i + "--");
                } else {
                    outFile.append("-" + i + "-");
                }
            }

            outFile.append("-\n");

            for (int i = 1; i < numProc + 1; i++) {
                outFile.append("P(" + i + ")");
                if (i < 10) {
                    outFile.append(" ");
                }

                for (int j = 0; j < currentTime; j++) {
                    if (scheduleTable[i][j] > 0) {
                        outFile.append("| " + scheduleTable[i][j] + " ");
                    } else {
                        outFile.append("| " + "-" + " ");
                    }

                    if (scheduleTable[i][j] < 10) {
                        outFile.append(" ");
                    }
                }

                outFile.append("|\n");
            }

            outFile.append("\n\n");
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
            while (open != null && procUsed < numProc) {
                int availProc = findProcessor();
                if (availProc > 0) {
                    procUsed++;
                    Node newJob = popOpen();
                    processJob[availProc] = newJob.jobId;
                    processTime[availProc] = newJob.jobTime;
                    updateTable(availProc, newJob);
                }
            }
        }

        void updateTable(int proc, Node job) {
            for (int i = currentTime; i <= currentTime + job.jobTime; i++) {
                scheduleTable[proc][i] = job.jobId;
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

        boolean graphEmpty() {
            for (int i = 1; i < numNodes + 1; i++) {
                if (jobDone[i] == 0) {
                    return false;
                }
            }

            return true;
        }

        boolean checkCycle() {
            boolean procFinished = true;
            for (int i = 1; i < numProc + 1; i++) {
                if (processTime[i] > 0) {
                    procFinished = false;
                }
            }

            return open == null && !graphEmpty() && procFinished;
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
    }

    public static void main(String... argv) {
        try {
            inFile1 = new Scanner(new File(argv[0]));
            inFile2 = new Scanner(new File(argv[1]));
            outFile = new FileWriter(new File(argv[2]));

            int numNodes;

            numNodes = inFile1.nextInt();

            Scanner consoleIn = new Scanner(System.in);

            int numProc = 0;

            while (numProc <= 0) {
                System.out.println("Number of processors:");

                numProc = consoleIn.nextInt();
                if (numProc <= 0) {
                    System.out.println("Invalid number of processors.");
                }
            }

            if (numProc > numNodes) {
                numProc = numNodes;
            }

            Scheduler scheduler = new Scheduler(numNodes, numProc);
            scheduler.loadMatrix();
            scheduler.computeTotalJobTimes();

            while (!scheduler.graphEmpty()) {
                Node orphan;

                while ((orphan = scheduler.getUnmarkedOrphan()) != null) {
                    scheduler.insertOpen(orphan);
                }

                scheduler.printList();

                scheduler.populateTable();

                if (scheduler.checkCycle()) {
                    scheduler.printTable();

                    System.out.println("Cycle in the graph!");
                    outFile.append("Cycle in the graph!");

                    inFile1.close();
                    inFile2.close();
                    outFile.close();
                    return;
                }

                scheduler.printTable();

                scheduler.incrementTime();

                System.out.println("Current Time: " + scheduler.currentTime);
                System.out.print("jobMarked: [");
                for (int i = 1; i < numNodes + 1; i++) {
                    System.out.print(scheduler.jobMarked[i]);
                    if (i < numNodes) {
                        System.out.print(", ");
                    }
                }
                System.out.println("]");
                System.out.print("processTime: [");
                for (int i = 1; i < numProc + 1; i++) {
                    System.out.print(scheduler.processTime[i]);
                    if (i < numProc) {
                        System.out.print(", ");
                    }
                }
                System.out.print("]\n");
                System.out.print("processJob: [");
                for (int i = 1; i < numProc + 1; i++) {
                    System.out.print(scheduler.processJob[i]);
                    if (i < numProc) {
                        System.out.print(", ");
                    }
                }
                System.out.print("]\n");
                System.out.print("jobDone: [");
                for (int i = 1; i < numNodes + 1; i++) {
                    System.out.print(scheduler.jobDone[i]);
                    if (i < numNodes) {
                        System.out.print(", ");
                    }
                }
                System.out.print("]\n\n");
            }

            scheduler.printTable();

            inFile1.close();
            inFile2.close();
            outFile.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}