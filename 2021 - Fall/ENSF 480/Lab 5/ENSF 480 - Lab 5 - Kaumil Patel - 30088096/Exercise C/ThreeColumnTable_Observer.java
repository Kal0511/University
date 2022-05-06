/*
* File Name: ThreeColumnTable_Observer.java
* Assignment: Lab 5 
* Lab Section: B01
* Completed by: Kaumil Patel
* Submission Date: Nov 4, 2021
*/

import java.util.ArrayList;

public class ThreeColumnTable_Observer implements Observer{

	public ThreeColumnTable_Observer(DoubleArrayListSubject mydata) {
		mydata.registerObserver(this);
		update(mydata.getData());
	}

	@Override
	public void update(ArrayList<Double> arr) {
		int i = 0;
		System.out.println("\nNotification to Three-Column Table Observer: Data Changed:");
		while(i<arr.size()) {
			for(int n=0;n<3 && i<arr.size();n++) {
				System.out.print(arr.get(i) + " ");
				i++;
			}
			System.out.println();
		}
	}

}
