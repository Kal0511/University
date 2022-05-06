import java.nio.file.Paths;
import java.nio.file.Path;
import java.nio.file.Files;
import java.util.ArrayList;
import java.util.Scanner;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;

class FileIO {
	public static void main(String[] args) {
		ArrayList<String> lines = readFile(args[0]);
		for (int i = 0; i < lines.size(); i++) {
			System.out.println(lines.get(i));
		}
		writeFile("XXXXXXX", "XXXXXXX"); // test line (to be modified)
	}

	public static ArrayList<String> readFile(String path) {
		Path inputfile = Paths.get(path + ".txt");
		ArrayList<String> lines = new ArrayList<String>();
		try (BufferedReader reader = Files.newBufferedReader(inputfile)) {
			String line = null;
			while ((line = reader.readLine()) != null) {
				lines.add(line);
			}
		} catch (IOException x) {
			System.err.format("IOException: %s%n", x);
			System.out.println("Please input a valid file name.");
			System.exit(0);
		}
		return lines;
	}

	public static void writeFile(String path, String data) {
		Path outputfile = Paths.get(path + ".txt");
		try (BufferedWriter writer = Files.newBufferedWriter(outputfile)) {
			writer.write(data, 0, data.length());
		} catch (IOException x) {
			System.err.format("IOException: %s%n", x);
		}
	}

	public static void createFile(String path) {
		Path outputfile = Paths.get(path + ".txt");
		try (BufferedWriter writer = Files.newBufferedWriter(outputfile)) {
		} catch (IOException x) {
			System.err.format("IOException: %s%n", x);
		}
	}

	public static void appendFile(String path, String data) {
		try (BufferedWriter writer = new BufferedWriter(new FileWriter(path + ".txt", true))) {
			writer.write(data);
			writer.close();
		} catch (IOException x) {
			System.err.format("IOException: %s%n", x);
		}
	}

	public static String inputString() {
		Scanner input = new Scanner(System.in);
		String word = input.next();
		System.out.println();
		input.close();
		return word;
	}
}
