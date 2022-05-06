/*
* File Name: MyVector.java
* Assignment: Lab 5 
* Lab Section: B01
* Completed by: Kaumil Patel
* Submission Date: Nov 4, 2021
*/

import java.util.ArrayList;
import java.text.DecimalFormat;

public class MyVector< E extends Number & Comparable<E> > {
	private ArrayList<Item<E>> storageM;
	private Sorter<E> sorter;
	MyVector(int n){
		storageM = new ArrayList<Item<E>>(n);
	}
	MyVector(ArrayList<E> arr){
		storageM = new ArrayList<Item<E>>(arr.size());
		for(int i=0;i<arr.size();i++) {
			storageM.add(new Item<E>(arr.get(i)));
		}
	}
	public void add(Item<E> value) {
		storageM.add(value);
	}
	public void setSortStrategy(Sorter <E> s) {
		sorter = s;
	}
	public void performSort() {
		sorter.sort(storageM);
	}
	public void display() {
		for(int i=0;i<storageM.size();i++) {
			DecimalFormat df = new DecimalFormat("#.0");
			System.out.print(df.format(storageM.get(i).getItem()));
			System.out.println();
		}
	}
}
