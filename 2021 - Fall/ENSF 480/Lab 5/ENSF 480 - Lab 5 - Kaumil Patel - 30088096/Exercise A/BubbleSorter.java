/*
* File Name: BubbleSorter.java
* Assignment: Lab 5 
* Lab Section: B01
* Completed by: Kaumil Patel
* Submission Date: Nov 4, 2021
*/

import java.util.ArrayList;

public class BubbleSorter< E extends Number & Comparable<E> > extends Sorter<E>{

	@Override
	public void sort(ArrayList<Item<E>> arr) {
		for(int i=0;i<arr.size()-1;i++) {
			for(int j=i+1;j<arr.size();j++) {
				if(arr.get(j).lessThan(arr.get(i))) {
					Item<E> temp = arr.get(i);
					arr.set(i, arr.get(j));
					arr.set(j, temp);
				}
			}
		}
	}

}
