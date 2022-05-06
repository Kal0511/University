/**
@author     Kaumil Patel <a href="mailto:kaumil.patel@ucalgary.ca">kaumil.patel@ucalgary.ca</a>
@version    4.1
@since      4.0
*/


//SimpleArrays is an implementation of the characteristics specified  from exercise 4.1


import java.util.Arrays;

public class SimpleArrays {
	
	//SimpleArrays is a class with two constructors, a concat and a crop function
	//SimpleArrays has a array of strings

	private String[] arrayOfStrings;

	//constructor with one String argument which is used to fill the 4 element array
	public SimpleArrays(String fillWith) {
		arrayOfStrings = new String[4];
		Arrays.fill(arrayOfStrings, fillWith);
	}

	//constructor which fills the 4 element array with the default String ""
	public SimpleArrays() {
		arrayOfStrings = new String[4];
		Arrays.fill(arrayOfStrings, "Hello, ENSF 409");
	}

	//this function takes in one argument which indicated where to start the concatenation of the array
	//if the argument is out of bounds then the concatenation start at index 0
	public String arrayConcat(int start) {
		if(start >= arrayOfStrings.length || start < 0) {
			start = 0;
		}
		return arrayCrop(start, arrayOfStrings.length-1);
	}
	
	//this function concats the string array starts from the first index
	public String arrayConcat() {
		return arrayCrop(0, arrayOfStrings.length-1);
	}
	
	//arrayCrop takes in two arguments a start and stop position which are inclusive. Then it concats those indexs from the string array and return the concated string
	//if the arguments are the same them it returns "Match"
	//if the arguments are out of bound it returns "Fail"
	public String arrayCrop(int start, int end) {
		if(end < start) {
			int temp = start;
			start = end;
			end = temp;
		}
		if(start<0 || end >= arrayOfStrings.length) {
			return "Fail";
		}
		if(start == end) {
			return "Match";
		}
		String cropped = "";
		while(true){
			if(start == end){
				cropped += arrayOfStrings[start];
				break;
			}
			cropped += arrayOfStrings[start]+"#";
			start++;
		}
		return cropped;
	}

	public static void main(String[] args) {
		SimpleArrays test1 = new SimpleArrays();
		SimpleArrays test2 = new SimpleArrays("hi man");
		
		System.out.println(test1.arrayConcat(1));
		System.out.println(test2.arrayConcat(5));
		
		System.out.println(test1.arrayCrop(-1, 1));
		System.out.println(test2.arrayCrop(1,1));
		System.out.println(test1.arrayCrop(2, 3));
		System.out.println(test2.arrayCrop(2,5));
		System.out.println(test2.arrayCrop(3,2));
	}

}
