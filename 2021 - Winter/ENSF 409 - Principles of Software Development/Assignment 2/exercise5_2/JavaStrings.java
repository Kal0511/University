/**
@author     Kaumil Patel <a href="mailto:kaumil.patel@ucalgary.ca">kaumil.patel@ucalgary.ca</a>
@version    4.1
@since      4.0
*/


//JavaStrings is an implementation of the characteristics specified  from exercise 5.2

public class JavaStrings {

	//addTogether takes in two strings as arguments
	//then it trims the leading and trailing whitespaces from each String and are concated
	//lastly it returns the length of the concated string
	public static int addTogether(String str1, String str2) {
		return str1.trim().length() + str2.trim().length();
	}
	
	//idProcessing takes in three strings and one int as arguments
	//then the first letter of each string and the int are concated 
	//the string formed from concatination is returned
	public static String idProcessing(String firstName, String lastName, String petName, int year) {
		return firstName.substring(0,1) + lastName.substring(0,1) + petName.substring(0,1) + String.valueOf(year);
	}
	
	//secretCode takes in one string as an argument
	//then the (a,e,i,o,u) in the string are replaced with "z"
	//lastly the first three letter of the scrambled string are returned
	public static String secretCode(String ingredients) {
		return ingredients.replaceAll("[a|e|i|o|u]", "z").substring(0,3);
	}
	
	public static void main(String[] args) {
		System.out.println(addTogether("kaumil","Patel"));
		System.out.println(idProcessing("kaumil","Patel","dog",2548));
		System.out.println(secretCode("tomato"));
	}

}
