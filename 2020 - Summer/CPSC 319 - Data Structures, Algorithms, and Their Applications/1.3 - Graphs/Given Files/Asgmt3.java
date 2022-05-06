
class Asgmt3 {
	public static void main(String[] args) {
		int[][] matrix = AdjacencyMatrixParser.parseFile(inputFileName); // inputFileName should be provided by the user (refer to the asgmt specs)
		
		DijkstrasAlgorithm.dijkstra(matrix, 0);
	}

}