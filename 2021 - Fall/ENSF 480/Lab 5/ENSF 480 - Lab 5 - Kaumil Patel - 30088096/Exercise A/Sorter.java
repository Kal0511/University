/*
* File Name: Sorter.java
* Assignment: Lab 5 
* Lab Section: B01
* Completed by: Kaumil Patel
* Submission Date: Nov 4, 2021
*/

import java.util.ArrayList;

public abstract class Sorter< E extends Number & Comparable<E> > {
	abstract public void sort(ArrayList<Item<E>> arr);

}
