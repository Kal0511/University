/**
 * @author 		Kaumil Patel <a href="mailto:kaumil.patel@ucalgary.ca">kaumil.patel@ucalgary.ca</a>
 * @version 	1.1
 * @since 		1.0
 */

/*
 * MyBook is an implementation of the UML diagram provided in exercise 12.4
 * This program can be used to make 4 different types of books - Novel, Anthology, Nonfiction, and Classic
 */

public class MyBook {
	public static void main(String[] args) {
		//Nonfiction, Category, Paperback
		// var history = new Nonfiction();
		// var deweyOne = new Category();
		// deweyOne.setCategory("History");
		// var deweyTwo = new Category();
		// deweyTwo.setCategory("French");
		// deweyTwo.setSuperCategory(deweyOne);
		// deweyOne.setSubCategory(deweyTwo);
		// var deweyThree = new Category();
		// deweyThree.setCategory("Revolution");
		// deweyThree.setSuperCategory(deweyTwo);
		// deweyTwo.setSubCategory(deweyThree);
		// System.out.println(deweyThree.sort());
		// history.setDeweyClassification(deweyThree);
		// System.out.println(history.topic());
		// System.out.println(history.coverArt());
		//Series, Novel
		// var anne = new Author("Anne Rice", "USA", 60);
		// System.out.println(anne.write());
		// var interviewWithVampire = new Novel();
		//interviewWithVampire.setTheAuthor(anne);
		// System.out.println(interviewWithVampire.coverArt());
		// System.out.println(interviewWithVampire.genre());
		//Anthology, Story
		// var vampireInParis = new Story();
		//vampireInParis.setTheAuthor(anne);
		// System.out.println(vampireInParis.plot());
		// var vampiresEverywhere = new Anthology();
		// System.out.println(vampiresEverywhere.storyOrder());
	}
}

abstract class Book {
	
	// Abstract class Book which is extended on in other classes to increase functionality 
	// Book also includes getters, setters and 2 constructors 
	
	private String isbn;
	private int publicationYear;
	private int pages;

	public String getIsbn() {
		return this.isbn;
	}

	public void setIsbn(String isbn) {
		this.isbn = isbn;
	}

	public int getPublicationYear() {
		return this.publicationYear;
	}

	public void setPublicationYear(int publicationYear) {
		this.publicationYear = publicationYear;
	}

	public int getPages() {
		return this.pages;
	}

	public void setPages(int pages) {
		this.pages = pages;
	}

	public Book() {
		this.isbn = "";
		this.publicationYear = 0;
		this.pages = 0;
	}

	public Book(String isbn, int pages) {
		this.isbn = isbn;
		this.publicationYear = 0;
		this.pages = pages;
	}
}

abstract class Paperback extends Book {

	// Paperback extends Book with method coverArt which returns a string
	// Includes constructors for super class

	public String coverArt() {
		return "Method coverArt called from Paperback";
	}

	public Paperback() {
		super();
	}

	public Paperback(String isbn, int pages) {
		super(isbn, pages);
	}
}

abstract class Hardcover extends Book {

	// Hardcover extends Book with void method binding 
	// Includes constructors for super class

	public abstract void binding();

	public Hardcover() {
		super();
	}

	public Hardcover(String isbn, int pages) {
		super(isbn, pages);
	}
}

abstract class Fiction extends Paperback {

	// Fiction extends Paperback with method genre and abstract method coverArt where both return a string
	// Includes constructors for super class

	public abstract String coverArt();

	public String genre() {
		return "Method genre called from Fiction";
	}

	public Fiction() {
		super();
	}

	public Fiction(String isbn, int pages) {
		super(isbn, pages);
	}
}

final class Nonfiction extends Paperback {
	
	// Nonfiction extends Paperback with method topic which returns a string
	// Includes constructors for super class
	// Includes getters, setters
	
	private Category deweyClassification;

	public Category getDeweyClassification() {
		return this.deweyClassification;
	}

	public void setDeweyClassification(Category deweyClassification) {
		this.deweyClassification = deweyClassification;
	}

	public String topic() {
		return "Method topic called from Nonfiction";
	}

	public Nonfiction() {
		super();
	}

	public Nonfiction(String isbn, int pages) {
		super(isbn, pages);
	}
}

final class Novel extends Fiction {
	
	// Novel extends Fiction with method theme which returns a string
	// Includes constructors for super class
	// Includes getters, setters
	
	private Author[] theAuthor;
	private Series[] mySeries;

	public Author[] getTheAuthor() {
		return this.theAuthor;
	}

	// The size of array theAuthor must be between 1 and 10
	public void setTheAuthor(Author[] theAuthor) {
		if (theAuthor.length < 1 || theAuthor.length > 10) {
			System.out.println("Error: theAuthor size must be between 1 and 10");
		} else {
			this.theAuthor = theAuthor;
		}
	}

	public Series[] getMySeries() {
		return this.mySeries;
	}

	// The size of array mySeries must be between 1 and 10
	public void setMySeries(Series[] mySeries) {
		if (mySeries.length < 1 || mySeries.length > 10) {
			System.out.println("Error: mySeries size must be between 1 and 10");
		} else {
			this.mySeries = mySeries;
		}
	}

	public String theme() {
		return "Method theme called from Novel";
	}

	public Novel() {
		super();
	}

	public Novel(String isbn, int pages) {
		super(isbn, pages);
	}

	final public String coverArt() {
		return "Method coverArt called from Novel";
	}
}

final class Anthology extends Fiction {
	
	// Anthology extends Fiction with method storyOrder which returns a string
	// Includes constructors for super class
	// Includes getters, setters
	
	private Story[] story;

	public Story[] getStory() {
		return this.story;
	}

	// The size of array Story must be between 5 and 10
	public void setStory(Story[] story) {
		if (story.length < 5 || story.length > 10) {
			System.out.println("Error: story size must be between 5 and 10");
		} else {
			this.story = story;
		}
	}

	public String storyOrder() {
		return "Method storyOrder called from Anthology";
	}

	public Anthology() {
		super();
	}

	public Anthology(String isbn, int pages) {
		super(isbn, pages);
	}

	final public String coverArt() {
		return "Method coverArt called from Anthology";
	}
}

class Category {
	
	// Category includes method sort which returns a string
	// Includes getters, setters
	
	private Category subCategory;
	private Category superCategory;
	private String category;

	public Category getSubCategory() {
		return this.subCategory;
	}

	public void setSubCategory(Category subCategory) {
		this.subCategory = subCategory;
	}

	public Category getSuperCategory() {
		return this.superCategory;
	}

	public void setSuperCategory(Category superCategory) {
		this.superCategory = superCategory;
	}

	public String getCategory() {
		return this.category;
	}

	public void setCategory(String category) {
		this.category = category;

	}

	public String sort() {
		return "Method sort called from Category";
	}
}

class Story {
	
	// Story includes method plot which returns a string
	// Includes getters, setters
	
	private Author[] theAuthor;

	public Author[] getTheAuthor() {
		return this.theAuthor;
	}

	// The size of array theAuthor must be between 1 and 10
	public void setTheAuthor(Author[] theAuthor) {
		if (theAuthor.length < 1 || theAuthor.length > 10) {
			System.out.println("Error: theAuthor size must be between 1 and 10");
		} else {
			this.theAuthor = theAuthor;
		}
	}

	public String plot() {
		return "Method plot called from Story";
	}
}

class Series {
	
	// Series includes method theme which returns a string
	// Includes getters, setters
	
	private String seriesName;

	public String getSeriesName() {
		return this.seriesName;
	}

	public void setSeriesName(String seriesName) {
		this.seriesName = seriesName;
	}

	public String theme() {
		return "Method theme called from Series";
	}
}

class Author {
	
	// Author includes method write which returns a string
	// Includes getters, setters
	
	private String name = "Unknown";
	private String address;
	private int age;

	public String getName() {
		return this.name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public String getAddress() {
		return this.address;
	}

	public void setAddress(String address) {
		this.address = address;
	}

	public int getAge() {

		return this.age;
	}

	public void setAge(int age) {
		this.age = age;
	}

	public Author() {
		this.address = "";
		this.age = 0;
	}

	public Author(String name, String address, int age) {
		this.name = name;
		this.address = address;
		this.age = age;
	}

	public String write() {
		return "Method write called from Author";
	}
}

final class Classic extends Hardcover {
	
	// Classic extends Hardcover with method createNotes which returns a string
	// Includes constructors for super class
	// Includes getters, setters
	
	private int origPubYear = 1860;
	private Author[] theAuthor;
	private Publisher[] bookPublisher;

	public int getOrigPubYear() {
		return this.origPubYear;
	}

	public void setOrigPubYear(int origPubYear) {
		this.origPubYear = origPubYear;
	}

	public Author[] getTheAuthor() {
		return this.theAuthor;
	}

	// The size of array theAuthor must be between 0 and 10
	public void setTheAuthor(Author[] theAuthor) {
		if (theAuthor.length > 10) {
			System.out.println("Error: theAuthor size must be between 0 and 10");
		} else {
			this.theAuthor = theAuthor;
		}
	}

	public Publisher[] getBookPublisher() {
		return this.bookPublisher;
	}

	// The size of array bookPublisher must be between 1 and 10
	public void setBookPublisher(Publisher[] bookPublisher) {
		if (bookPublisher.length < 1 || bookPublisher.length > 10) {
			System.out.println("Error: bookPublisher size must be between 1 and 10");
		} else {
			this.bookPublisher = bookPublisher;
		}
	}

	public String createNotes() {
		return "Method createNotes called from Classic";
	}

	final public void binding() {
		System.out.println("Method binding called from Classic");
	}

	public Classic() {
		super();
	}

	public Classic(String isbn, int pages) {
		super(isbn, pages);
	}
}

class Publisher {
	
	// Publisher includes method printLeatterHead which returns a string
	// Includes getters, setters
	
	private String name;
	private String address;
	private Classic[] classicsCatalog;

	public String getName() {
		return this.name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public String getAddress() {
		return this.address;
	}

	public void setAddress(String address) {
		this.address = address;
	}

	public Classic[] getClassicsCatalog() {
		return this.classicsCatalog;
	}

	// The size of array classicsCatalog must be between 0 and 10
	public void setClassicsCatalog(Classic[] classicsCatalog) {
		if (classicsCatalog.length > 10) {
			System.out.println("Error: bookPublisher size must be between 0 and 10");
		} else {
			this.classicsCatalog = classicsCatalog;
		}
	}

	Publisher(String name, String address) {
		this.name = name;
		this.address = address;
		this.classicsCatalog = new Classic[0];
	}

	public String printLetterHead() {
		return "Method printLetterHead called from Publisher";
	}
}

class Contract {
	
	// Publisher includes method printContract which returns a string
	// Includes getters, setters
	
	private String date;
	private Publisher publisherInfo;
	private Author authorInfo;

	public String getDate() {
		return this.date;
	}

	public void setData(String data) {
		this.date = data;
	}

	public Publisher getPublisherInfo() {
		return this.publisherInfo;
	}

	public void setPublisherInfo(Publisher publisherInfo) {
		this.publisherInfo = publisherInfo;
	}

	public Author getAuthorInfo() {
		return this.authorInfo;
	}

	public void setAuthorInfo(Author authorInfo) {
		this.authorInfo = authorInfo;
	}

	Contract(String date, Publisher publisherInfo, Author authorInfo) {
		this.date = date;
		this.publisherInfo = publisherInfo;
		this.authorInfo = authorInfo;
	}

	public String printContract() {
		return "Method printContract called from Contract";
	}
}