/**
 * @author 		Kaumil Patel <a href="mailto:kaumil.patel@ucalgary.ca">kaumil.patel@ucalgary.ca</a>
 * @version 	1.1
 * @since 		1.0
 */

/*
 * LogParser is an implementation of the UML diagram provided in exercise 14.3
 */


import java.util.ArrayList;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class MyOutput {
	public static void main(String args[]) {
		String[] exampleLog = exampleLog();
		var logfile = new ParseLogFile(exampleLog);

		var line = logfile.getLine(0);
		System.out.println("Log line 0: " + line.getLogLine());

		var ip = line.getIPv4();
		System.out.println("IPv4: " + ip.getIP());

		var dt = line.getDateTime();
		System.out.println("Day: " + dt.getDay());
		System.out.println("Month: " + dt.getMonth());
		System.out.println("Month (named): " + dt.monthToString());
		System.out.println("Year: " + dt.getYear());
		System.out.println("Hour: " + dt.getHour());
		System.out.println("Minute: " + dt.getMinute());
		System.out.println("Second: " + dt.getSecond());

		var act = line.getAction();
		System.out.println("Action: " + act.getAction());

		var dev = line.getDevice();
		System.out.println("Device: " + dev.getDevice());

		var loc = line.getLocation();
		System.out.println("Room: " + loc.getRoom());
		System.out.println("Building: " + loc.getBuilding());

		System.out.println();

		line = logfile.getLine(6);
		System.out.println("Log line 6: " + line.getLogLine());
		System.out.println(line.getIPv4().getFormatted());
		System.out.println(line.getDateTime().getFormatted());
		System.out.println(line.getAction().getFormatted());
		System.out.println(line.getDevice().getFormatted());
		System.out.println(line.getLocation().getFormatted());

		System.out.println("\nExample of toLog() output: " + Months.AUG.toLog());
		System.out.println("\nExample regex (for DateTime): " + dt.getRegex() + "\n");

		for (int i = 0; i < logfile.getLog().size(); i++) {
			var log = logfile.getLine(i);
			System.out.println("Log line: " + log.getLogLine());
		}
	}

	// Contains example data
	public static String[] exampleLog() {
		String[] log = { "81.220.24.207 - - [2/Mar/2020:10:05:44] \"END sprinkler (Visitor entrance - Building A)\"",
				"81.220.24.207 - - [2/Mar/2020:10:05:26] \"ENABLE cooling system (Secured room - Building A)\"",
				"81.220.24.207 - - [2/Mar/2020:10:05:39] \"START heating system (Hall - Central)\"",
				"81.220.24.207 - - [2/Mar/2020:10:05:52] \"ENABLE door lock (Visitor entrance - Building B)\"",
				"81.220.24.207 - - [2/Mar/2020:10:05:21] \"TEST cooling system (Entrance - Building B)\"",
				"66.249.73.135 - - [17/May/2020:01:05:17] \"TEST fan (Secured room - Airport location)\"",
				"46.105.14.53 - - [17/May/2020:11:05:42] \"TEST cooling system (Secured room - Airport location)\"",
				"46.105.14.53 - - [17/May/2020:11:05:42] \"HI cooling system (Secured room - Airport location)\"",
				"218.30.103.62 - - [17/May/2020:11:05:11] \"START sprinkler (Secured room - Airport location)\"",
				"218.30.103.62 - - [17/May/2020:11:05:46] \"DISABLE fan (Control room - Central)\"",
				"218.30.103.62 - - [17/May/2020:11:05:45] \"START door lock (Secured room - Building A)\"",
				"66.249.73.135 - - [27/Jun/2020:11:05:31] \"TEST sprinkler (Hall - Building B)\"" };
		return log;
	}
}

enum Months {
	JAN {
		public int toInt() {
			return 1;
		}

		public String toLog() {
			return "Jan";
		}

		public String toString() {
			return "January";
		}
	},
	FEB {
		public String toString() {
			return "February";
		}

		public int toInt() {
			return 2;
		}

		public String toLog() {
			return "Feb";
		}
	},
	MAR {
		public String toString() {
			return "March";
		}

		public int toInt() {
			return 3;
		}

		public String toLog() {
			return "Mar";
		}
	},
	APR {
		public String toString() {
			return "April";
		}

		public int toInt() {
			return 4;
		}

		public String toLog() {
			return "Apr";
		}
	},
	MAY {
		public String toString() {
			return "May";
		}

		public int toInt() {
			return 5;
		}

		public String toLog() {
			return "May";
		}
	},
	JUN {
		public String toString() {
			return "June";
		}

		public int toInt() {
			return 6;
		}

		public String toLog() {
			return "Jun";
		}
	},
	JUL {
		public String toString() {
			return "July";
		}

		public int toInt() {
			return 7;
		}

		public String toLog() {
			return "Jul";
		}
	},
	AUG {
		public String toString() {
			return "August";
		}

		public int toInt() {
			return 8;
		}

		public String toLog() {
			return "Aug";
		}
	},
	SEP {
		public String toString() {
			return "September";
		}

		public int toInt() {
			return 9;
		}

		public String toLog() {
			return "Sep";
		}
	},
	OCT {
		public String toString() {
			return "October";
		}

		public int toInt() {
			return 10;
		}

		public String toLog() {
			return "Oct";
		}
	},
	NOV {
		public String toString() {
			return "November";
		}

		public int toInt() {
			return 11;
		}

		public String toLog() {
			return "Nov";
		}
	},
	DEC {
		public String toString() {
			return "December";
		}

		public int toInt() {
			return 12;
		}

		public String toLog() {
			return "Dec";
		}
	};

	public int toInt() {
		return this.toInt();
	}

	public String toLog() {
		return this.toLog();
	}
}

enum Actions {
	END, ENABLE, START, TEST, DISABLE;
}

class ParseLogFile {
	private ArrayList<ParseLine> log;

	public ParseLogFile(String[] array) {
		log = new ArrayList<ParseLine>();
		for (int i = 0; i < array.length; i++) {
			try {
				Actions.valueOf(new Action(array[i]).getAction());
				this.log.add(new ParseLine(array[i]));
			} catch (IllegalArgumentException e) {
			}
		}
	}

	public ParseLine getLine(int index) {
		return this.log.get(index);
	}

	public ArrayList<ParseLine> getLog() {
		return this.log;
	}
}

class ParseLine {
	private String LOGLINE;
	private Location LOCATION;
	private Device DEVICE;
	private Action ACTION;
	private DateTime DATETIME;
	private IPv4 IPV4;

	public ParseLine(String line) {
		this.IPV4 = new IPv4(line);
		this.LOCATION = new Location(line);
		this.DEVICE = new Device(line);
		this.ACTION = new Action(line);
		this.DATETIME = new DateTime(line);
		this.LOGLINE = this.IPV4.getIP() + " - - " + this.DATETIME.getLogFormatted() + " \"" + this.ACTION.getAction()
				+ " " + this.DEVICE.getDevice() + " (" + this.LOCATION.getRoom() + " - " + this.LOCATION.getBuilding()
				+ ")\"";
	}

	public IPv4 getIPv4() {
		return this.IPV4;
	}

	public String getLogLine() {
		return this.LOGLINE;
	}

	public Location getLocation() {
		return this.LOCATION;
	}

	public Device getDevice() {
		return this.DEVICE;
	}

	public Action getAction() {
		return this.ACTION;
	}

	public DateTime getDateTime() {
		return this.DATETIME;
	}
}

class DateTime implements FormattedOutput{
	private int DAY;
	private int MONTH;
	private Months month;
	private int YEAR;
	private int HOUR;
	private int MINUTE;
	private int SECOND;
	private String REGEX = "([0-9]{1,2})\\/(\\w+)\\/([0-9]+)\\:([0-9]{1,2})\\:([0-9]{1,2})\\:([0-9]{1,2})";
	private Pattern PATTERN = Pattern.compile(REGEX);

	public DateTime(String dateTime) {
		Matcher matcher = PATTERN.matcher(dateTime);
		if (matcher.find()) {
			this.DAY = Integer.parseInt(matcher.group(1));
			this.month = Months.valueOf(matcher.group(2).toUpperCase());
			this.MONTH = month.toInt();
			this.YEAR = Integer.parseInt(matcher.group(3));
			this.HOUR = Integer.parseInt(matcher.group(4));
			this.MINUTE = Integer.parseInt(matcher.group(5));
			this.SECOND = Integer.parseInt(matcher.group(6));
		}
	}

	public String getFormatted() {
		return "Day: " + this.DAY + ", Month: " + this.MONTH + ", Year: " + this.YEAR + ", Hour: " + this.HOUR
				+ ", Minute: " + this.MINUTE + ", Second: " + this.SECOND;
	}

	public String monthToString() {
		return month.toString();
	}

	public int getDay() {
		return this.DAY;
	}

	public int getMonth() {
		return this.MONTH;
	}

	public int getYear() {
		return this.YEAR;
	}

	public int getHour() {
		return this.HOUR;
	}

	public int getMinute() {
		return this.MINUTE;
	}

	public int getSecond() {
		return this.SECOND;
	}

	public String getRegex() {
		return this.REGEX;
	}

	public String getLogFormatted() {
		return "[" + this.DAY + "/" + this.month.toLog() + "/" + this.YEAR + ":" + this.HOUR + ":" + this.MINUTE + ":"
				+ this.SECOND + "]";
	}
}

class IPv4 implements FormattedOutput{
	private String IP;
	private String REGEX = "[0-9]{1,3}[.][0-9]{1,3}[.][0-9]{1,3}[.][0-9]{1,3}";
	private Pattern PATTERN = Pattern.compile(REGEX);

	public IPv4(String ip) {
		Matcher matcher = PATTERN.matcher(ip);
		if (matcher.find()) {
			this.IP = matcher.group();
		}
	}

	public String getFormatted() {
		return "IPv4: " + this.IP;
	}

	public String getIP() {
		return this.IP;
	}

	public String getRegex() {
		return this.REGEX;
	}
}

class Action implements FormattedOutput{
	private String ACTION;
	private String REGEX = "[A-Z][A-Z]+";
	private Pattern PATTERN = Pattern.compile(REGEX);

	public Action(String action) {
		Matcher matcher = PATTERN.matcher(action);
		if (matcher.find()) {
			this.ACTION = matcher.group();
		}
	}

	public String getFormatted() {
		return "Action: " + this.ACTION;
	}

	public String getAction() {
		return ACTION;
	}

	public String getRegex() {
		return REGEX;
	}
}

class Device implements FormattedOutput{
	private String DEVICE;
	private String REGEX = "([a-z][a-z ]+)[ ][(]";
	private Pattern PATTERN = Pattern.compile(REGEX);

	public Device(String device) {
		Matcher matcher = PATTERN.matcher(device);
		if (matcher.find()) {
			this.DEVICE = matcher.group(1);
		}
	}

	public String getFormatted() {
		return "Device: " + this.DEVICE;
	}

	public String getDevice() {
		return this.DEVICE;
	}

	public String getRegex() {
		return this.REGEX;
	}

}

class Location implements FormattedOutput{
	private String ROOM;
	private String BUILDING;
	private String REGEX = "([\\w ]+) - ([\\w ]+)";
	private Pattern PATTERN = Pattern.compile(REGEX);

	public Location(String location) {
		Matcher matcher = PATTERN.matcher(location);
		if (matcher.find()) {
			this.ROOM = matcher.group(1);
			this.BUILDING = matcher.group(2);
		}
	}

	public String getFormatted() {
		return "Room: " + this.ROOM + ", Building: " + this.BUILDING;
	}

	public String getRoom() {
		return this.ROOM;
	}

	public String getBuilding() {
		return this.BUILDING;
	}

	public String getRegex() {
		return this.REGEX;
	}
}

interface FormattedOutput{
	String getFormatted();
}