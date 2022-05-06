package edu.ucalgary.ensf409;

/**
 * @author Kaumil Patel <a href=
 *         "mailto:kaumil.patel@ucalgary.ca">kaumil.patel@ucalgary.ca</a>
 * @version 1.1
 * @since 1.0
 */

/*
 * StringProcessor is an implementation of exercise 18.2
 */

public class StringProcessor {

	public static void main(String[] args) {
		StringProcessor one = new StringProcessor("SeCret");
		System.out.println(one.addTogetherMirror("  fewfwef CAT  "));
		System.out.println(StringProcessor.idProcessing("Kaumil", "Kal", "Kal", 2021));
		System.out.println(StringProcessor.idProcessing("Kaumil", "Ba", "Kal", 2021));
		System.out.println("k".length() >= 2);
		System.out.println(one.secretCode(10));
	}

	private final String storedString;

	public StringProcessor(String input) {
		this.storedString = new String(input);
	}

	public String addTogetherMirror(String inputString) {
		String combined = (storedString.trim() + inputString.trim()).toLowerCase();
		return new StringBuilder(combined).reverse().toString();
	}

	public static void firstCharUpper(String str) {
		if (!Character.isUpperCase(str.charAt(0))) {
			throw new IllegalArgumentException(str + ": First character is not capital");
		} else if (str.length() == 2 && !Character.isLetter(str.charAt(1))) {
			throw new IllegalArgumentException(str + ": Not valid");
		}
	}

	public static void rightLength(String str) {
		if (str.length() < 2 || str.length() > 26) {
			throw new IllegalArgumentException(str + ": Length is not inbetween 2 and 26");
		}
	}

	public static String idProcessing(String firstName, String lastName, String petName, int year) {
		if (year > 2021) {
			throw new IllegalArgumentException(year + ": Is more than 2021");
		}

		rightLength(firstName);
		rightLength(lastName);
		rightLength(petName);

		firstCharUpper(firstName);
		firstCharUpper(lastName);
		firstCharUpper(petName);

		String petID = new String(String.valueOf(firstName.charAt(0)) + String.valueOf(lastName.charAt(0))
				+ String.valueOf(petName.charAt(0)) + String.valueOf(year));
		return petID;
	}

	public String secretCode(int offset) {
		String encodedString = new String();
		for (int i = 0; i < storedString.length(); i++) {
			if (Character.isUpperCase(storedString.charAt(i))) {
				int newUnicode = alphabetWrap(storedString.charAt(i) + offset - 65) + 65;
				encodedString = encodedString.concat(String.valueOf((char) newUnicode));
			} else if (Character.isLowerCase(storedString.charAt(i))) {
				int newUnicode = alphabetWrap(storedString.charAt(i) + offset - 97) + 97;
				encodedString = encodedString.concat(String.valueOf((char) newUnicode));
			} else {
				encodedString = encodedString.concat(String.valueOf(storedString.charAt(i)));
			}
		}
		return encodedString;
	}

	public String getStoredString() {
		return this.storedString;
	}

	public int alphabetWrap(int alpha) {
		alpha %= 26;
		if (alpha < 0) {
			alpha += 26;
		}
		return alpha;
	}
}
