import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.charset.Charset;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.List;

public class Lox {
    static boolean hadError = false;

    public static void main(String[] args) throws IOException {
        if (args.length > 1) { // Error
            System.out.println("Usage: jlox [script]");
            System.exit(64);
        } else if (args.length == 1) { // Read code from the file path provided
            runFile(args[0]);
        } else { // Run jlox and now you can type your code
            runPrompt();
        }
    }

    private static void runFile(String path) throws IOException {
        /* 
        * Find the file using Paths
        *  Open the file and read everything inside using Files
        *  Store the result in a byte array.
        */
        byte[] bytes = Files.readAllBytes(Paths.get(path));
        run(new String(bytes, Charset.defaultCharset()));

        // Error in the code! Do not run this.
        if (hadError) System.exit(65);
    }

    private static void runPrompt() throws IOException {
        // Reads a stream of inputs. From where? System.in i.e terminal.
        InputStreamReader input = new InputStreamReader(System.in);
        BufferedReader reader = new BufferedReader(input);

        // While the user is entering the values
        while(true) {
            System.out.print("> "); // This is where the user's cursor will type
            String line = reader.readLine();
            if (line == null) return; // finish. Ctrl+D typed.
            run(line); // execute it.
            hadError = false; // reset the boolean
        }
    }

    private static void run(String source) {
        // Provide the code to scanner to read from. Our own scanner
        Scanner scanner = new Scanner(source);

        // Get tokenize and store the tokens in a list
        List<Token> tokens = scanner.scanTokens();

        for (Token token : tokens) {
            System.out.println(token); // print the tokens to see the results
        }
    }

    static void error(int line, String message) {
        report(line, "", message);
    }

    private static void report(int line, String where, String message) {
        System.err.println(
            "[line " + line + "] Error" + where + ": " + message
        );
        hadError = true;
    }
}