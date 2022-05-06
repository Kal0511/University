/*
* File Name: DoubleArrayListSubject.java
* Assignment: Lab 5 
* Lab Section: B01
* Completed by: Kaumil Patel
* Submission Date: Nov 4, 2021
*/

import java.util.ArrayList;

public class DoubleArrayListSubject implements Subject{
	private ArrayList<Double> data;
	private ArrayList<Observer> observers;
	
	public DoubleArrayListSubject(){
		data = new ArrayList<Double>();
		observers = new ArrayList<Observer>();
	}
	
	public void addData(Double value) {
		data.add(value);
		notifyAllObservers();
	}
	
	public void setData(Double value, int index) {
		data.set(index, value);
		notifyAllObservers();
	}
	
	public ArrayList<Double> getData() {
		return data;
	}
	
	public void populate(double[] arr) {
		data = new ArrayList<Double>(arr.length);
		for(int i=0;i<arr.length;i++) {
			data.add(arr[i]);
		}
		notifyAllObservers();
	}
	
	public void display() {
		for(int i=0;i<data.size();i++) {
			System.out.print(data.get(i)+" ");
		}
		System.out.println();
	}
	
	public void remove(Observer observer) {
		observers.remove(observer);
	}
	
	@Override
	public void registerObserver(Observer observer) {
		observers.add(observer);
	}
	
	@Override
	public void removeObserver(Observer observer) {
		observers.remove(observer);
	}
	
	@Override
	public void notifyAllObservers() {
		for(int i=0;i<observers.size();i++) {
			observers.get(i).update(data);
		}
	}
	
}
