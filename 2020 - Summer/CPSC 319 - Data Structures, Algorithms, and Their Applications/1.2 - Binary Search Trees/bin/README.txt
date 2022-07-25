This is an implementation of a binary search tree which takes in text files and stores the words and there frequency in the tree.

Main Function(menu):
	Contains a menu with all the functions that can be used to manipulate and view the tree. There are nine options provided to the user.
	1.	Load from file: this option loads a text file into the tree, the user is prompted to input a filename. The option uses the loadFile() function.
	2.	Search tree: the user is prompted to input a word which is searched in the tree and the frequency is given to the user. The search() function is used.
	3.	Insert word: the user is prompted to input a word which is inserted in the tree and the word has been inserted into the tree based on the binary search tree rules. The insert() function is used.
	4.	Remove word: the user is prompted to input a word which is removed in the tree and word has been removed into the tree based on the binary search tree rules. The remove() function is used.
	5.	Pre-Order traversal: the user is provided with the tree in pre-order.
	6.	In-Order traversal: the user is provided with the tree in in-order.
	7.	Post-Order traversal: the user is provided with the tree in post-order.
	8.	Tree Details: the user is provided with the total words in the tree, the number of unique words in the tree, and the user is also give the words that occur most in the tree. The treeDetails() function is used.
	9.	Exit: the program is terminated.

compile command
	javac bst.java
	
run command
	java bst
	