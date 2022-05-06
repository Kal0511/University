package edu.ucalgary.ensf409;

import java.sql.*;

/**
 * @author Kaumil Patel <a href=
 *         "mailto:kaumil.patel@ucalgary.ca">kaumil.patel@ucalgary.ca</a>
 * @version 1.1
 * @since 1.0
 */

/*
 * Registration is an implementation of exercise 20.3
 */

public class Registration {

	public final String DBURL; // store the database url information
	public final String USERNAME; // store the user's account username
	public final String PASSWORD; // store the user's account password
	private Connection dbConnect;

	/*
	 * This constructor initializes the three public final String data members
	 * called DBURL, USERNAME, and PASSWORD
	 */
	Registration(String _DBURL, String _USERNAME, String _PASSWORD) {
		DBURL = _DBURL;
		USERNAME = _USERNAME;
		PASSWORD = _PASSWORD;
	}

	/*
	 * This method gets BDURL
	 */
	public String getDburl() {
		return DBURL;
	}

	/*
	 * This method gets USERNAME
	 */
	public String getUsername() {
		return USERNAME;
	}

	/*
	 * This method gets PASSWORD
	 */
	public String getPassword() {
		return PASSWORD;
	}

	/*
	 * This method has no arguments and returns nothing Creates a connection to the
	 * database
	 */
	public void initializeConnection() {
		try {
			dbConnect = DriverManager.getConnection(DBURL, USERNAME, PASSWORD);
		} catch (SQLException e) {
			e.printStackTrace();
		}
	}

	// Some example test data is shown here. This is not a full list of all possible
	// tests (i.e. competitors under the age of 5, over 18, etc.)
	// Tests will be done on a database with the same table names/attributes, but
	// different data records.

	public static void main(String[] args) {

		Registration myJDBC = new Registration("jdbc:mysql://localhost/competition", "Marasco", "ensf409");
		myJDBC.initializeConnection();

		System.out.println(myJDBC.selectAllNames("competitor"));
		/*
		 * Example: Williams, Sophie Warren, Harper
		 */

		System.out.println(myJDBC.selectAllNames("teacher"));
		/*
		 * Example: Estrada, Ethan Lee, Jasmine
		 */

		System.out.println(myJDBC.showStudios());
		/*
		 * Example: Harmony Inc. Melody Time Music Mastery
		 */

		myJDBC.insertNewCompetitor("123", "Smyth", "Ali", 15, "Oboe", "0023");
		myJDBC.registerNewTeacher("0987", "Marasco", "Emily", "403-222-5656", "Marasco Music", "587-222-5656",
				"123 Main Street NW");

		myJDBC.deleteCompetitor("123");
		myJDBC.deleteTeacher("0987");

	}

	/*
	 * This method has one String which is the teacher's id for its argument and
	 * returns nothing Deletes the specified teacher from the Teacher table
	 */
	public void deleteTeacher(String ID) {
		try {
			String query = "DELETE FROM teacher WHERE teacherID = ?";
			PreparedStatement myStmt = dbConnect.prepareStatement(query);
			myStmt.setString(1, ID);
			myStmt.executeUpdate();
			myStmt.close();
		} catch (SQLException ex) {
			ex.printStackTrace();
		}
	}

	/*
	 * This method has one String which is the competitor's id for its argument and
	 * returns nothing Deletes the specified competitor from the Competitor table
	 */
	public void deleteCompetitor(String ID) {
		try {
			String query = "DELETE FROM competitor WHERE competitorID = ?";
			PreparedStatement myStmt = dbConnect.prepareStatement(query);
			myStmt.setString(1, ID);
			myStmt.executeUpdate();
			myStmt.close();
		} catch (SQLException ex) {
			ex.printStackTrace();
		}
	}

	/*
	 * This method takes in seven String arguments which appear in Teacher and
	 * Studio tables and returns nothing Insert the teacher into the Teacher table
	 * If the specified teacher is already registered, throw an
	 * IllegalArgumentException If the teacher’s studio is not listed in the Studio
	 * table, add the studio’s information to the Studio table
	 */
	public void registerNewTeacher(String TeacherID, String LName, String FName, String Phone, String StudioName,
			String StudioPhone, String StudioAddress) {
		try {
			Statement myStmt = dbConnect.createStatement();
			ResultSet results = myStmt
					.executeQuery("SELECT COUNT(name) FROM studio WHERE Name = \"" + StudioName + "\"");
			results.next();
			if (results.getInt("COUNT(name)") == 0) {
				registerNewStudio(StudioName, StudioPhone, StudioAddress);
			}
			myStmt.close();
		} catch (SQLException ex) {
			ex.printStackTrace();
		}
		try {
			String query = "INSERT INTO teacher (TeacherID, LName, FName, Phone, StudioName) VALUES (?,?,?,?,?)";
			PreparedStatement myStmt = dbConnect.prepareStatement(query);
			myStmt.setString(1, TeacherID);
			myStmt.setString(2, LName);
			myStmt.setString(3, FName);
			myStmt.setString(4, Phone);
			myStmt.setString(5, StudioName);
			myStmt.executeUpdate();
			myStmt.close();
		} catch (SQLIntegrityConstraintViolationException e) {
			throw new IllegalArgumentException("Error: Teacher already exists");
		} catch (SQLException ex) {
			ex.printStackTrace();
		}
	}

	/*
	 * This method takes in three String arguments which appear in the Studio table
	 * and returns nothing Insert the studio into the Studio table
	 */
	public void registerNewStudio(String StudioName, String StudioPhone, String StudioAddress) {
		try {
			String query = "INSERT INTO studio (name, phone, address) VALUES (?,?,?)";
			PreparedStatement myStmt = dbConnect.prepareStatement(query);
			myStmt.setString(1, StudioName);
			myStmt.setString(2, StudioPhone);
			myStmt.setString(3, StudioAddress);
			myStmt.executeUpdate();
			myStmt.close();
		} catch (SQLException ex) {
			ex.printStackTrace();
		}
	}

	/*
	 * This method takes in six String arguments which appear in the Competitor
	 * table and returns nothing Insert the competitor into the Competitor table If
	 * the specified teacher is not included in the Teacher table, throw an
	 * IllegalArgumentException If the competitor is not between the ages of 5 and
	 * 18, throw an IllegalArgumentException
	 */
	public void insertNewCompetitor(String CompetitorID, String LName, String FName, int Age, String Instrument,
			String TeacherID) {
		if (Age < 5 || Age > 18) {
			throw new IllegalArgumentException("Error:  Competitor age is not between 5 and 18");
		}
		try {
			Statement myStmt = dbConnect.createStatement();
			ResultSet results = myStmt.executeQuery("SELECT COUNT(*) FROM teacher WHERE teacherID = " + TeacherID);
			results.next();
			if (results.getInt("COUNT(*)") == 0) {
				throw new IllegalArgumentException("Error: Teacher does not exists");
			}
			myStmt.close();
		} catch (SQLException ex) {
			ex.printStackTrace();
		}
		try {
			String query = "INSERT INTO competitor (CompetitorID, LName, FName, Age, Instrument, TeacherID) VALUES (?,?,?,?,?,?)";
			PreparedStatement myStmt = dbConnect.prepareStatement(query);
			myStmt.setString(1, CompetitorID);
			myStmt.setString(2, LName);
			myStmt.setString(3, FName);
			myStmt.setInt(4, Age);
			myStmt.setString(5, Instrument);
			myStmt.setString(6, TeacherID);
			myStmt.executeUpdate();
			myStmt.close();
		} catch (SQLException ex) {
			ex.printStackTrace();
		}
	}

	/*
	 * This method takes in zero arguments and returns a String containing a list of
	 * all names in the Studio table
	 */
	public String showStudios() {
		StringBuffer studios = new StringBuffer();
		try {
			Statement myStmt = dbConnect.createStatement();
			ResultSet results = myStmt.executeQuery("SELECT name FROM studio");
			results.next();
			while (true) {
				studios.append(results.getString("name"));
				if (results.next()) {
					studios.append("\n");
				} else {
					break;
				}
			}
			myStmt.close();
		} catch (SQLException e) {
			e.printStackTrace();
		}
		return studios.toString();
	}

	/*
	 * This method takes in zero arguments and returns a String containing any last
	 * and first names in the specified table
	 */
	public String selectAllNames(String tableName) {
		StringBuffer names = new StringBuffer();
		try {
			Statement myStmt = dbConnect.createStatement();
			ResultSet results = myStmt.executeQuery("SELECT fname, lname FROM " + tableName);
			results.next();
			while (true) {
				names.append(results.getString("lname") + ", " + results.getString("fname"));
				if (results.next()) {
					names.append("\n");
				} else {
					break;
				}
			}
			myStmt.close();
		} catch (SQLException e) {
			e.printStackTrace();
		}
		return names.toString();
	}
}
