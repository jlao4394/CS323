#include <iostream>
#include <fstream>

int main(int argc, char *argv[]) {
    std::cout << argv[0];
    std::ifstream in(argv[1]);
    int rows, cols, minVal, maxVal;

    in >> rows >> cols >> minVal >> maxVal;

    int p;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            in >> p;

            if (p >= 1) {
                std::cout << i << " " << j << '\n';
            }
        }
    }

    in.close();

    return 0;
}
