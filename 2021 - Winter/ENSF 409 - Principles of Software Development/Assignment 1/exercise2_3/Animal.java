/**
@author     Kaumil Patel <a href="mailto:kaumil.patel@ucalgary.ca">kaumil.patel@ucalgary.ca</a>
@version    1.3
@since      1.0
*/

/*
Animal is an implementation of the UML class diagram from exercise 2.3
*/

public class Animal {
	
	// The Animal class has two private variables, legs and color
	// The class also has functions to get and set the legs and color of an Animal
	private int legs = 4;
	private String color;
		
	//Animal constructor that takes in a new color and leg size 
	public Animal(String newColor, int newLegs) {
		this.color = newColor;
		this.legs = newLegs;
	}
	
	//Animal constructor that takes in a new color and has the default leg size of 4
	public Animal(String newColor){
		this.color = newColor;
	}
	
	public int getLegs(){
		return this.legs;
	}
	public void setLegs(int newLegs){
		this.legs = newLegs;
	}
	
	public String getColor(){
		return this.color;
	}
	public void setColor(String newColor){
		this.color = newColor;
	}
	
	public static void main(String[] args) {
		Animal cat = new Animal("Brown", 5);
		System.out.println("The cat's fur color is: " + cat.getColor());
		System.out.println("The cat's leg size is: " + cat.getLegs());
		cat.setLegs(7);
		System.out.println("The cat grew, new leg size is: " + cat.getLegs());
	}
}
