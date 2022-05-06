class Asgmt3 {
	public static void main(String[] args) {
		System.out.print("Please enter file name: ");
		String fileName = FileIO.inputString();
		int[][] matrix = AdjacencyMatrixParser.parseFile(fileName);

		String[][] pathData = DijkstrasAlgorithm.dijkstra(matrix, 0);

		FileIO.createFile(fileName + "_dijkstra_output");
		FileIO.appendFile(fileName + "_dijkstra_output", "Vertex\t\tDistance\t Path\n");
		for (int i = 0; i < pathData.length; i++) {
			if (pathData[i][0] != null) {
				FileIO.appendFile(fileName + "_dijkstra_output",
						pathData[i][0] + "\t\t" + pathData[i][1] + "\t\t\t" + pathData[i][2] + "\n");
			}
		}
		System.out.print("\n\nDijkstra shortest path for file " + fileName + " has been outputted to " + fileName + "_dijkstra_output");
	}
}