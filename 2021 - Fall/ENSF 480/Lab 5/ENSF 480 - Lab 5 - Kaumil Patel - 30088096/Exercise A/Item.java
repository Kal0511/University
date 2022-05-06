/*
* File Name: Item.java
* Assignment: Lab 5 
* Lab Section: B01
* Completed by: Kaumil Patel
* Submission Date: Nov 4, 2021
*/

class Item <E extends Number & Comparable<E> >{
	private E item;
	public Item(E value) {
		item = value;
	}
	
	public void setItem(E value){
		item = value;
	}
	
	public E getItem(){
		return item;
	}
	
	public boolean lessThan(Item<E> rhs){  
		if(item.compareTo(rhs.item) < 0)  
			return true;
		else  
			return false;  
	}  
}
