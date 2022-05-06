import java.util.ArrayList;

public class AdjacencyMatrixParser {
	public static void main(String[] args) {
	}
	public static int[][] parseFile(String inputFileName) {
		ArrayList<String> file = FileIO.readFile(inputFileName);
		for (int i = 0; i < file.size(); i++) {
			if (file.get(i).charAt(0) == '#' || "".equals(file.get(i))) {
				file.remove(i);
				i--;
			}
		}
		int size = file.size();
		int[][] matrix = new int[size][size];
		int col = 0;
		for (String line : file) {
			String[] num = line.trim().split(" +");
			for (int row = 0; row < num.length; row++) {
				if ("".equals(num[row])) {
					break;
				}
				matrix[row][col] = Integer.parseInt(num[row]);
			}
			col++;
		}
		return matrix;
	}
}
