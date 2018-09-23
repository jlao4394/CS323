#include <iostream>
#include <fstream>
#include <cmath>

struct xyCoord {
    int xCoord;
    int yCoord;
};

class Point {
public:
    int xCoord;
    int yCoord;
    int label;
};

class Kmean {
    int kCluster;
    int numPts;
    Point *ptList;
    xyCoord *kCentroids;

public:
    int changeFlag;

    Kmean(int kCluster, int numPts) {
        this->kCluster = kCluster;
        this->numPts = numPts;
        changeFlag = 1;
        ptList = new Point[numPts];
        kCentroids = new xyCoord[kCluster + 1];
    }

    void loadPtList(std::ifstream *in) {
        for (int i = 0; i < numPts; i++) {
            Point p;
            *in >> p.xCoord;
            *in >> p.yCoord;

            ptList[i] = p;
        }
    }

    void assignLabel() {
        for (int i = 0; i < numPts; i++) {
            ptList[i].label = (i % kCluster) + 1;
        }
    }

    void computeCentroid() {
        int *sumCnt = new int[kCluster + 1];
        int *sumX = new int[kCluster + 1];
        int *sumY = new int[kCluster + 1];
        for (int i = 1; i < kCluster + 1; i++) {
            sumCnt[i] = 0;
            sumX[i] = 0;
            sumY[i] = 0;
        }

        int ptIndex = 0;
        while (ptIndex < numPts) {
            int label = ptList[ptIndex].label;
            sumX[label] += ptList[ptIndex].xCoord;
            sumY[label] += ptList[ptIndex].yCoord;
            sumCnt[label]++;
            ptIndex++;
        }

        int clusterIndex = 1;
        while (clusterIndex <= kCluster) {
            kCentroids[clusterIndex].xCoord = sumX[clusterIndex] / sumCnt[clusterIndex];
            kCentroids[clusterIndex].yCoord = sumY[clusterIndex] / sumCnt[clusterIndex];
            clusterIndex++;
        }
    }

    void clusterCheck() {
        double *dist = new double[kCluster + 1];
        for (int i = 1; i < kCluster + 1; i++) {
            dist[i] = 0.0;
        }

        int ptIndex = 0;

        while (ptIndex < numPts) {
            int minLabel = ptList[ptIndex].label;
            double minDist = computeDist(ptList[ptIndex], kCentroids[minLabel]);
            int clusterIndex = 1;

            while (clusterIndex <= kCluster) {
                dist[clusterIndex] = computeDist(ptList[ptIndex], kCentroids[clusterIndex]);
                if (dist[clusterIndex] < minDist) {
                    minDist = dist[clusterIndex];
                    minLabel = clusterIndex;
                }
                clusterIndex++;
            }

            if (ptList[ptIndex].label != minLabel) {
                ptList[ptIndex].label = minLabel;
                changeFlag++;
            }

            ptIndex++;
        }
    }

    double computeDist(Point p1, xyCoord p2) {
        double powX = std::pow(p1.xCoord - p2.xCoord, 2);
        double powY = std::pow(p1.yCoord - p2.yCoord, 2);
        return std::sqrt(powX + powY);
    }

    void printPtList(std::ofstream *out) {
        Point *pt;
        for (int i = 0; i < numPts; i++) {
            pt = &ptList[i];
            *out << '(' << pt->xCoord << ", " << pt->yCoord << ", " << pt->label << ")" << std::endl;
        }
        *out << std::endl;
    }

    class Image {
        int numRows;
        int numCols;
        int **imgAry;
    public:
        Image(int numRows, int numCols) {
            this->numRows = numRows;
            this->numCols = numCols;

            imgAry = new int *[numRows];
            for (int i = 0; i < numRows; i++) {
                imgAry[i] = new int[numCols];

                for (int j = 0; j < numCols; j++) {
                    imgAry[i][j] = 0;
                }
            }
        }

        void mapPoint2Image(Kmean *kmean) {
            for (int i = 0; i < kmean->numPts; i++) {
                imgAry[kmean->ptList[i].xCoord][kmean->ptList[i].yCoord] = kmean->ptList[i].label;
            }
        }

        void prettyPrintImgAry(std::ofstream *out) {
            for (int i = 0; i < numRows; i++) {
                for (int j = 0; j < numCols; j++) {

                    if (imgAry[i][j] > 0) {
                        *out << imgAry[i][j];
                    } else {
                        *out << ' ';
                    }
                }

                *out << std::endl;
            }

            *out << std::endl;
        }
    };
};

int main(int argc, char *argv[]) {
    std::ifstream in(argv[1]);
    std::ofstream out(argv[2]);

    int numRows, numCols, numPts;
    in >> numRows >> numCols >> numPts;

    int kCluster;
    std::cout << "Please provide a value for k." << std::endl;
    std::cin >> kCluster;

    Kmean kmean(kCluster, numPts);
    Kmean::Image image(numRows, numCols);

    kmean.loadPtList(&in);
    kmean.assignLabel();
    kmean.printPtList(&out);

    while (kmean.changeFlag > 0) {
        image.mapPoint2Image(&kmean);
        image.prettyPrintImgAry(&out);
        kmean.changeFlag = 0;
        kmean.computeCentroid();
        kmean.clusterCheck();
    }

    in.close();
    out.close();

    return 0;
}