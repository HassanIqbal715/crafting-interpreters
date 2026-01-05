public class Main {
    private static int add(int a, int b) {
        return a + b;
    }

    public static void main(String[] args) {
        System.out.println("Hello World");
        for (int i = 0; i < 10; i++) {
            System.out.println(add(i, i+1));
        }
    }
}