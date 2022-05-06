/*
* File Name: FiveRowsTable_Observer.java
* Assignment: Lab 5 
* Lab Section: B01
* Completed by: Kaumil Patel
* Submission Date: Nov 4, 2021
*/

import java.util.ArrayList;

public class FiveRowsTable_Observer implements Observer{

	public FiveRowsTable_Observer(DoubleArrayListSubject mydata) {
		mydata.registerObserver(this);
		update(mydata.getData());
	}

	@Override
	public void update(ArrayList<Double> arr) {
		int i = 0;
		System.out.println("\nNotification to Five-Rows Table Observer: Data Changed:");
		while(i<arr.size()) {
			for(int n=0;n<3;n++) {
				if(i+n*5>=arr.size()) {
					System.out.println();
					return;
				}
				System.out.print(arr.get(i+n*5) + " ");
			}
			i++;
			System.out.println();
		}
	}

}
