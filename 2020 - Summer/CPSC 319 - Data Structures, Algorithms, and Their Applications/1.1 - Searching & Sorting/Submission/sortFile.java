import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Scanner;

public class sortFile {
	
	private ArrayList<String> fileContent = new ArrayList<String>();
	private String fileName;
	
	public static void main(String[] args) {
		sortFile sort = new sortFile();
		sort.openFile();
		sort.selectionSort();
		sort.outputFile();
	}
	
	public void openFile() {
		Scanner input = new Scanner(System.in);
		System.out.println("Enter Filename: ");
		fileName = input.nextLine();
		System.out.println("Filename is: " + fileName);
		input.close();
		try {
			BufferedReader reader = new BufferedReader(new FileReader(fileName));
							
			String word = reader.readLine();
			
			while(word!=null) {
				fileContent.add(word);
				word = reader.readLine();				
			}	
			reader.close();
		} catch (IOException ioe) {
			System.out.println(ioe.getMessage());
			System.exit(0);
		}		
	}

	public void selectionSort() {
		int low = 0;
		int n = -1;
		for (int i = 0; i < fileContent.size(); i++) {
			low = i;
			for (int j = i + 1; j < fileContent.size(); j++) {
				n = -1;
				String temp;
				if (fileContent.get(low).length() < fileContent.get(j).length()) {
					temp = fileContent.get(low);
				} else {
					temp = fileContent.get(j);
				}
				for (int l = 0; l < temp.length(); l++) {
					if ((int) fileContent.get(low).charAt(l) != (int) fileContent.get(j).charAt(l)) {
						n = l;
						break;
					}
				}
				if (fileContent.get(j).length() < fileContent.get(low).length() && n==-1) {
					low = j;
				}
				if(n==-1) {
					n=0;
				}
				if ((int) fileContent.get(low).charAt(n) > (int) fileContent.get(j).charAt(n)) {
					low = j;
				}
			}
			swap(i, low);
		}
	}

	public void swap(int i, int j) {
		String temp;
		temp = fileContent.get(i);
		fileContent.set(i, fileContent.get(j));
		fileContent.set(j, temp);
	}

	public String selectionSortString(String s) {
		int low = 0;
		for (int i = 0; i < s.length(); i++) {
			low = i;
			for (int j = i + 1; j < s.length(); j++) {
				if ((int) s.charAt(low) > (int) s.charAt(j)) {
					low = j;
				}
			}
			s = swap(s, i, low);
		}
		return s;
	}

	public String swap(String s, int i, int j) {
		char temp;
		temp = s.charAt(i);
		s = replace(s, i, s.charAt(j));
		s = replace(s, j, temp);
		return s;
	}

	public String replace(String s, int i, char c) {
		char[] temp = s.toCharArray();
		temp[i] = c;
		String output = new String(temp);
		return output;
	}

	public boolean isAnagram(String s1, String s2) {
		if (s1.length() != s2.length()) {
			return false;
		}
		s1 = selectionSortString(s1);
		s2 = selectionSortString(s2);
		if (s1.compareTo(s2) != 0) {
			return false;
		}
		return true;
	}

	public void outputFile() {
		fileName = fileName.substring(0, 9) + "_out.txt";
		BufferedWriter writer;
		try {
			writer = new BufferedWriter(new FileWriter(fileName, false));
			for (int i = 0; i < fileContent.size(); i++) {
				System.out.println();
				System.out.print(fileContent.get(i) + " ");
				writer.append(fileContent.get(i) + " ");
				for (int j = i + 1; j < fileContent.size(); j++) {
					if (isAnagram(fileContent.get(i), fileContent.get(j))) {
						System.out.print(fileContent.get(j) + " ");
						writer.append(fileContent.get(j) + " ");
						fileContent.remove(j);
						j--;
					}
				}
				writer.append("\n");
			}
			writer.close();
			System.out.println();
			System.out.println();
			System.out.print("File Content had been sorted and written to file: " + fileName);
		} catch (IOException ioe) {
			System.out.println(ioe.getMessage());
			System.exit(0);
		}
	}
}
