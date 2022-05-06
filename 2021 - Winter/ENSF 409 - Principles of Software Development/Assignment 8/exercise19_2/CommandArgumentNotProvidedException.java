package edu.ucalgary.ensf409;

public class CommandArgumentNotProvidedException extends Exception {

	private static final long serialVersionUID = -3263952272665412021L;

	public CommandArgumentNotProvidedException() {
		super("No argument not found");
	}
}
