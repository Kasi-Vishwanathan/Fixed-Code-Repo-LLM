public class qm {
    public static void main(String[] args) {
        Integer[] arr = {1, 2, 3};
        printArray(arr);
    }

    private static void printArray(Integer[] arr) {
        for (int i = 0; i < arr.length; i++) {
            System.out.println(arr[i]);
        }
    }
}