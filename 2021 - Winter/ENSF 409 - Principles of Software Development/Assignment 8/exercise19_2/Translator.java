package edu.ucalgary.ensf409;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.util.Formatter;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

class Translator {
	private String code;
	private TranslationText obj;

	/*
	 * getTranslation() getter method
	 */
	public TranslationText getTranslation() {
		return obj;
	}

	/*
	 * translate() Accepts a month number (e.g., 1 for January), a day number (e.g.,
	 * 31 for the 31st), and a year. Throws an IllegalArgumentException if monthNum
	 * or dayNum is not valid. Returns the formatted sentence as a String. Notice
	 * that the String containing formatting uses numbered arguments - this is
	 * because some languages will put the words in the sentence in a different
	 * order, but the translate() method must be able to work without knowledge of
	 * the language structure. Note: You do not have to check if a day is valid for
	 * a particular month/year; February 31 or February 29, 2021 can both be
	 * accepted, but out of range values e.g., month 15 day 40, are not valid and
	 * should be handled with an IllegalArgumentException.
	 */
	public String translate(int month, int day, int year) {
		if (day > 31 || day < 1) {
			throw new IllegalArgumentException("The day is invalid");
		}
		if (month > 12 || month < 1) {
			throw new IllegalArgumentException("The month is invalid");
		}
		Formatter format = new Formatter();
		String temp = format.format(obj.getSentence(), obj.getDay(day - 1), obj.getMonth(month - 1), year).toString();
		format.close();
		return temp;
	}

	/*
	 * Constructor Accepts a String of a two-letter language code, dash, and
	 * two-letter region code, e.g., te-IN and throws an IllegalArgumentException if
	 * the language and region code are not in the correct format. Language codes
	 * are ISO 639-1 and region codes are ISO 3166, but this method only checks the
	 * format of the String, not the validity of the codes. It calls
	 * importTranslation().
	 */
	public Translator(String languageCode) {
		code = languageCode;
		if (checkCode()) {
			importTranslation();
		} else {
			throw new IllegalArgumentException("The format of the code is invalid");
		}
	}

	/*
	 * importTranslation() Calls deserialize() if the appropriate file exists,
	 * otherwise calls importFromText(). No arguments. Returns void.
	 */
	public void importTranslation() {
		File file = new File(code + ".ser");
		if (file.exists()) {
			deserialize();
		} else {
			try {
				importFromText();
				serialize();
			} catch (ArgFileNotFoundException e) {
				e.printStackTrace();
			}
		}
	}

	/*
	 * importFromText() Reads in from a the two-letter language code, dash,
	 * two-letter region code text file, in the form of ab-XY.txt, and instantiates
	 * a TranslationText object with the data. It can throw I/O exceptions. Throw a
	 * custom ArgFileNotFoundException when the file isn't found. We expect the .txt
	 * file to be in a valid format. The file is expected to be in the same
	 * directory. The files en-US.txt and el-GR.txt are examples of a valid .txt
	 * files. They will always consist of the month names, one per line, followed by
	 * the day names, one per line, followed by the sentence containing formatting
	 * strings. This is the last line in the file. You cannot make any assumptions
	 * about what will appear on each line, only that each line will contain only
	 * one data element, and that it will not contain an empty line. No arguments.
	 * Returns void.
	 */
	public void importFromText() throws ArgFileNotFoundException {
		String[] months = new String[12];
		String[] days = new String[31];
		String sentence;
		try {
			BufferedReader reader = new BufferedReader(new FileReader(code + ".txt"));
			for (int i = 0; i < 12; i++) {
				months[i] = reader.readLine();
			}
			for (int i = 0; i < 31; i++) {
				days[i] = reader.readLine();
			}
			sentence = reader.readLine();
			obj = new TranslationText(days, months, sentence);
			reader.close();
		} catch (FileNotFoundException e) {
			throw new ArgFileNotFoundException();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	/*
	 * serialize() Creates a serialized object file of the TranslationText object,
	 * with the name format la-CO.ser, where la is the two-letter language code and
	 * CO is the two-letter region code. An example of a serialized object file can
	 * be found in the exercise directory as es-BO.ser I/O exceptions can be thrown.
	 * No arguments. Returns void.
	 */
	public void serialize() {
		try {
			ObjectOutputStream output = new ObjectOutputStream(new FileOutputStream(code + ".ser"));
			output.writeObject(obj);
			output.close();
		} catch (IOException e) {
			System.err.println("Error opening file");
		}
	}

	/*
	 * deserialize() Creates a TranslationText object from a .ser file. The files
	 * are named xx-YY.ser, where xx is the two-letter language code and YY is the
	 * two- letter region code. es-bo.ser is an example. It can throw I/O
	 * exceptions. No arguments. Returns void.
	 */
	public void deserialize() {
		try {
			ObjectInputStream input = new ObjectInputStream(new FileInputStream(code + ".ser"));
			obj = (TranslationText) input.readObject();
			input.close();
		} catch (IOException e) {
			System.err.println("Error opening file");
		} catch (Exception e) {
			System.err.println("Generic error handler");
			e.printStackTrace();
		}
	}

	public boolean checkCode() {
		Pattern pattern = Pattern.compile("[a-z]{2}-[A-Z]{2}");
		Matcher matcher = pattern.matcher(code);
		return matcher.find();
	}
}
