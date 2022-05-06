import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.Scanner;

public class bst {
	
	public class node{
		private String word;
		private int count;
		private node left;
		private node right;
		public node() {
			word = null;
			count = 0;
			left = null;
			right = null;
		}
		public node(String w) {
			word = w;
			count = 1;
			left = null;
			right = null;
		}		
	}
	
	private node root;
	private String fileName;
	
	bst(){
		root = null;
		fileName = null;
	}
	
	public static void main(String[] args) {
		bst tree = new bst();
		while(true) {
			tree.mainMenu();
			switch(inputNumber()) {
			  default:
				  System.out.println("Invalid Input - Please Try Again");
				  System.out.println();
				  break;
			  case 1:
				  tree.loadFile();
				  tree.treeDetails();
				  System.out.println();
				  break;
			  case 2:
				  System.out.print("Input search word: ");
				  System.out.println("Found " + tree.search(inputString()) + " times");
				  System.out.println();
				  break;
			  case 3:
				  System.out.print("Input insert word: ");
				  tree.insert(inputString());
				  break;
			  case 4:
				  System.out.print("Input remove word: ");
				  tree.remove(inputString());
				  break;
			  case 5:
				  System.out.println("Tree Pre-Order: ");
				  tree.preOrder(tree.root);
				  System.out.println("\n");
				  break;
			  case 6:
				  System.out.println("Tree In-Order: ");
				  tree.inOrder(tree.root);
				  System.out.println("\n");
				  break;
			  case 7:
				  System.out.println("Tree Post-Order: ");
				  tree.postOrder(tree.root);
				  System.out.println("\n");
				  break;
			  case 8:
				  System.out.println("Tree Details: ");
				  tree.treeDetails();
				  System.out.println();
				  break;
			  case 9:
				  System.out.println("Thank You - Program Closed");
				  System.exit(0);
			}
		}
	}
	
	public void mainMenu() {
		System.out.println("Binary Search Tree");
		System.out.println("1. Load from file");
		System.out.println("2. Search tree");
		System.out.println("3. Insert word");
		System.out.println("4. Remove word");
		System.out.println("5. Pre-Order traversal");
		System.out.println("6. In-Order traversal");
		System.out.println("7. Post-Order traversal");
		System.out.println("8. Tree Details");
		System.out.println("9. Exit");
		System.out.print("Please pick one of the following: ");
	}
	
	public static int inputNumber() {
		Scanner input = new Scanner(System.in);
	        char ch = input.next().charAt(0);
	        if(ch>='0'||ch<='9') {
		        System.out.println();
		        return ch-48;
	        }
			return 0;
	}
	
	public static String inputString() {
		Scanner input = new Scanner(System.in);
        String word = input.next();
        System.out.println();
        return word;
	}
	
	public void loadFile() {
		System.out.print("Enter Filename: ");
		fileName = inputString() + ".txt";
		try {
			BufferedReader reader = new BufferedReader(new FileReader(fileName));
							
			String word = reader.readLine();
			
			while(word!=null) {
				String[] words = word.split("-|\\ ");
				for(String single : words) {
					single = single.toLowerCase();
					
					StringBuffer remove = new StringBuffer();
					 for (int k = 0; k < single.length(); k++) {
					     if(Character.isLetter(single.charAt(k))) {
					    	 remove.append(single.charAt(k));    
					     }
					 }
					 single = remove.toString();
					
				insert(single);
				}
				word = reader.readLine();				
			}	
			reader.close();
		} catch (IOException ioe) {
			System.out.println(ioe.getMessage()+"\n");
		}		
	}
	
	public int stringCompare(String str1, String str2) { 
        int len1 = str1.length(); 
        int len2 = str2.length(); 
        
        int min = Math.min(len1, len2); 
        for (int i = 0; i < min; i++) { 
            int ch1 = (int)str1.charAt(i); 
            int ch2 = (int)str2.charAt(i); 
            if (ch1 != ch2) { 
                return ch1 - ch2; 
            } 
        } 
        if (len1 != len2) { 
            return len1 - len2; 
        } else {
        	return 0; 
        }
    } 
	
	public int search(String find) {
		return searchRecursion(root, find);
	}
	
	public int searchRecursion(node n, String find) {
		if(n == null) {
			return 0;
		}
		if(stringCompare(n.word, find)==0) {
			return n.count;
		}
		if(stringCompare(find, n.word)<0) {
			return searchRecursion(n.left, find);
		} else {
			return searchRecursion(n.right, find);
		}
	}
	
	public void remove(String remove) {
		root = removeRecursion(root, remove);
	}
	
	public node removeRecursion(node n, String remove) {
		if(n == null) {
			return n;
		}
		if(stringCompare(remove, n.word)<0) {
			n.left = removeRecursion(n.left, remove);
		} else if(stringCompare(remove, n.word)>0) {
			n.right = removeRecursion(n.right, remove);
		} else {
			if (n.left == null) {
                return n.right; 
			} else if (n.right == null) {
                return n.left; 
			}
            n.word = minWord(n.right); 
            n.count = minCount(n.right);
            n.right = removeRecursion(n.right, n.word); 
		}
		return n;
	}
	
	String minWord(node n) { 
        String min = n.word; 
        while (n.left != null) { 
            min = n.left.word; 
            n = n.left; 
        } 
        return min; 
    } 
	
	int minCount(node n) { 
        int min = n.count; 
        while (n.left != null) { 
            min = n.left.count; 
            n = n.left; 
        } 
        return min; 
    } 
	
	public void insert(String insert) {
		root = insertRecursion(root, insert);
	}
	
	public node insertRecursion(node n, String insert) {
		if(n == null) {
			return new node(insert);
		}
		if(stringCompare(insert, n.word)<0) {
			n.left = insertRecursion(n.left, insert);
		} else if(stringCompare(insert, n.word)>0) {
			n.right = insertRecursion(n.right, insert);
		} else {
			n.count++;
		}
		return n;
	}
	
	public void inOrder(node print) {
		if(print == null) {
			return;
		}
		inOrder(print.left);
		System.out.printf("%s ", print.word);
		inOrder(print.right);
	}
	
	public void preOrder(node print) {
		if(print == null) {
			return;
		}
		System.out.printf("%s ", print.word);
		preOrder(print.left);
		preOrder(print.right);
	}
	
	public void postOrder(node print) {
		if(print == null) {
			return;
		}
		postOrder(print.left);
		postOrder(print.right);
		System.out.printf("%s ", print.word);
	}
	
	public int totalWords() {
		int total = 0;
		total = totalWordsRecursion(root, total);
		return total;
	}
	
	public int totalWordsRecursion(node n, int total) {
		if(n == null) {
			return 0;
		}
		total += totalWordsRecursion(n.left, total) + totalWordsRecursion(n.right, total);
		total++;
		return total;
	}
	
	public int uniqueWords() {
		int unique = 0;
		unique = uniqueWordsRecursion(root, unique);
		return unique;
	}
	
	public int uniqueWordsRecursion(node n, int unique) {
		if(n == null) {
			return 0;
		}
		unique += uniqueWordsRecursion(n.left, unique) + uniqueWordsRecursion(n.right, unique);
		if(n.count==1) {
			unique++;
		}
		return unique;
	}
	
	public void mostOccurs() {
		int high = mostOccursRecursion(root, 0);
		System.out.print("Words Most Occured: ");
		if(high==0) {
			System.out.println("NONE");
		}
		System.out.println();
		printMostOccurs(root, high);
	}
	
	public int mostOccursRecursion(node n, int high) {
		if(n == null) {
			return high;
		}
		if(n.count>high) {
			high = n.count;
		}
		high = mostOccursRecursion(n.left, high);
		high = mostOccursRecursion(n.right, high);
		return high;
	}
	
	public void printMostOccurs(node n, int high) {
		if(n == null) {
			return;
		}
		if(n.count==high) {
			System.out.println(n.count + " - " + n.word);
		}
		printMostOccurs(n.left, high);
		printMostOccurs(n.right, high);
	}
	
	public void treeDetails() {
		  System.out.println("Total Words Stored: " + totalWords());
		  System.out.println("Number of Unique Words Strored: " + uniqueWords());
		  mostOccurs();
	}
}
