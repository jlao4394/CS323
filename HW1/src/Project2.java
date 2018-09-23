import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

public class Project2 {
    public static void main(String... args) throws FileNotFoundException {
        final Scanner scanner = new Scanner(new File(args[0]));
        final int rows = scanner.nextInt();
        final int cols = scanner.nextInt();
        final int minVal = scanner.nextInt();
        final int maxVal = scanner.nextInt();

        final int[][] points = new int[rows][cols];

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (scanner.nextInt() >= 1) {
                    System.out.println(i + " " + j);
                }
            }
        }

        scanner.close();
    }
}
