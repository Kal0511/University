/*
* File Name: InsertionSorter.java
* Assignment: Lab 5 
* Lab Section: B01
* Completed by: Kaumil Patel
* Submission Date: Nov 4, 2021
*/

import java.util.ArrayList;

public class InsertionSorter< E extends Number & Comparable<E> > extends Sorter<E> {

	@Override
	public void sort(ArrayList<Item<E>> arr) {
		for(int i=1;i<arr.size();i++) {
			Item<E> key = arr.get(i);
			int j=i-1;
			while(j>=0 && key.lessThan(arr.get(j))) {
				arr.set(j+1, arr.get(j));
				j--;
			}
			arr.set(j+1, key);
		}
	}
	
}