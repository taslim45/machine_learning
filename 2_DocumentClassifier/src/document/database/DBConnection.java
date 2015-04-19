package document.database;

import java.sql.*;

public class DBConnection {

	static Connection con = null;
	private static void createConnection()
	{
	    try {
	    	Class.forName("org.sqlite.JDBC");
	    	con = DriverManager.getConnection("jdbc:sqlite:doc_db.db");
	    } catch ( Exception e ) {
	    	System.err.println( e.getClass().getName() + ": " + e.getMessage() );
	      	System.exit(0);
	    }
	    System.out.println("Opened database successfully");
	}
	public static Connection getConnection()
	{
		createConnection();
		return con;
	}
}
