/*
* File Name: OneRow_Observer.java
* Assignment: Lab 5 
* Lab Section: B01
* Completed by: Kaumil Patel
* Submission Date: Nov 4, 2021
*/

import java.util.ArrayList;

public class OneRow_Observer implements Observer {
	
	public OneRow_Observer(DoubleArrayListSubject mydata) {
		mydata.registerObserver(this);
		update(mydata.getData());
	}

	@Override
	public void update(ArrayList<Double> arr) {
		int i = 0;
		System.out.println("\nNotification to One-Row Observer: Data Changed:");
		while (i < arr.size()) {
			System.out.print(arr.get(i) + " ");
			i++;
		}
	}

}
