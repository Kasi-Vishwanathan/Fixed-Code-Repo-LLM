public class LoopExample {
    public static void main(String[] args) {
        for (int i = 0; i < 5; i++) {
            int j = 0;
            while (j < 5) {
                System.out.printf("i=%d, j=%d, sum=%d%n", i, j, i + j);
                j += 2;
            }
        }
    }
}