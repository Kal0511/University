package edu.ucalgary.ensf409;

public class ArgFileNotFoundException extends Exception {

	private static final long serialVersionUID = -7518585690569998534L;

	public ArgFileNotFoundException() {
		super("File not found");
	}
}
