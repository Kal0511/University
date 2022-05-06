/**
@author     Kaumil Patel <a href="mailto:kaumil.patel@ucalgary.ca">kaumil.patel@ucalgary.ca</a>
@version    1.3
@since      1.0
*/

/*
Hello uses all of the primitive data types and also show an implementation of automatic and explicit casting
*/

public class Hello {
	public static void main(String[] args) {
		
		//primitive data types
		boolean present = true;
		char initial = 'K';
		byte date = 13;
		short year = 2020;
		int month = 1;
		long random = 587962;
		float pi = 3.14f;
		double e = 2.71;
		
		//automatic casting
		int autoInt = year;
		
		//explicit casting
		float explFloat = (float)e;
		
		System.out.println("boolean - Hello? " + present);
		System.out.println("char - My initial: " + initial);
		System.out.println("byte - Today's date: " + date);
		System.out.println("short - Current year: " + year);
		System.out.println("int - Current month: " + month);
		System.out.println("long - Random number: " + random);
		System.out.println("float - PI: " + pi);
		System.out.println("double - Euler's constant: " + e);
		
		System.out.println("Automatic short to int: " + autoInt);
		
		System.out.println("Explicit double to float: " + explFloat);
	}
}