Asgmt3 - Main Program
	The program asks for a filename. The file data is used to make a adjacency matrix which is used to find the dijkstra shortest path. The path info is outputted to "filename_dijkstra_output”.
	
AdjacencyMatrixParser
	Function parseFile() was created to read and initialize the adjacency matrix for the dijkstra() function.

changes made to FileIO
	Function createFile() was added which is used to create new files given a new file name.
	Function appendFile() was added which needs a filename and the string which is to be appended to the file.
	Function inputString() was add which asks the used for a input that is returned as a String.
	
	
changes made to DijkstrasAlgorithm
	Function getSolution() was added which is used to get the shortest path data made by dijkstra().
	Funtion getPath() was added which used to get the paths made by dijkstra().
	dijkstra was changed so it return the shortest path data.

compile command
	javac Asgmt3.java
	
run command
	java Asgmt3
	