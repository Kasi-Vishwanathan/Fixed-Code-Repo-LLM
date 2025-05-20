import java.util.LinkedList;
import java.util.Queue;

public class qm {
    public static void main(String[] args) {
        Queue<String> queue = new LinkedList<>();

        // Add elements
        for (int i = 0; i <= 10; i++) {
            queue.add(Integer.toString(i));
        }

        // Process elements
        while (!queue.isEmpty()) {
            System.out.println("Processed: " + queue.poll());
        }
        System.out.println("Queue size now is " + queue.size());
    }
}